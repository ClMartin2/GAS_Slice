#include "Knife.h"
#include "Components/BoxComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/StaticMeshComponent.h"
#include "DrawDebugHelpers.h"
#include "Math/Quat.h"
#include "../Library/GAS_Utils.h"
#include "GAS_Slice/Chain.h"

AKnife::AKnife()
{
	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));
	BoxCollision->OnComponentHit.AddDynamic(this, &AKnife::OnHit);
	RootComponent = BoxCollision;

	StaticMeshKnife_ = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh Knife"));
	StaticMeshKnife_->SetupAttachment(RootComponent);

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile Movement"));
	ProjectileMovement->UpdatedComponent = BoxCollision;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->MaxSpeed = MaxSpeed;
	
	Chain = CreateDefaultSubobject<UChildActorComponent>(FName(TEXT("Chain")));
	Chain->SetChildActorClass(ChainClass);
	Chain->SetupAttachment(StaticMeshKnife_);
}

void AKnife::Throw_Implementation(FVector DirectionThrowKnife, FVector NewCameraForward)
{
	BP_Chain->SetSimulatePhysics(false);
	IsAttached = false;
	ProjectileMovement->SetUpdatedComponent(GetRootComponent());
	ProjectileMovement->InitialSpeed = Speed;
	ProjectileMovement->Velocity = DirectionThrowKnife * Speed;
	CameraForward = NewCameraForward;
}

void AKnife::ResetKnife()
{
	BP_Chain->SetSimulatePhysics(false);
	StopMove();
	IsAttached = false;
	BoxCollision->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
}

void AKnife::StopMove_Implementation()
{
	ProjectileMovement->Velocity = FVector::ZeroVector;
	ProjectileMovement->InitialSpeed = 0;
}

void AKnife::SetChainPhySicsHit()
{
	BP_Chain->SetSimulatePhysics(true);
}

void AKnife::OnHit_Implementation(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
                                  FVector NormalImpulse, const FHitResult& Hit)
{
	GetWorldTimerManager().SetTimer(TimerHandleSetPhysicsHit, this, &AKnife::SetChainPhySicsHit, 0.1f, false);
	IsAttached = true;
	StopMove();
	HitRotate(Hit);
	ReplaceHitKnife(Hit);
	AttachToActor(OtherActor, FAttachmentTransformRules::KeepWorldTransform);
	BoxCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AKnife::HitRotate(const FHitResult& Hit)
{
	FVector HitNormal = Hit.ImpactNormal;
	FRotator HitNormalRotation = (-HitNormal).Rotation();

	FRotator NewRotation = FRotator(CameraForward.Rotation().Pitch, CameraForward.Rotation().Yaw,
	                                HitNormalRotation.Roll);

	SetActorRotation(NewRotation);
}

void AKnife::ReplaceHitKnife(const FHitResult& Hit)
{
	FHitResult HitResult(ForceInit);

	FCollisionQueryParams RV_TraceParams = FCollisionQueryParams(FName(TEXT("RV_Trace")), true, this);
	RV_TraceParams.bTraceComplex = true;

	float LengthBoxCollision = BoxCollision->GetScaledBoxExtent().X;
	FVector Start = GetActorLocation();
	FVector End = Start + GetActorForwardVector() * (LengthBoxCollision + 100);

	GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECollisionChannel::ECC_Visibility, RV_TraceParams);

	FVector ImpactLocation = HitResult.ImpactPoint;
	float DistanceBetweenMeshAndRootLocation = FVector::Distance(GetActorLocation(),
	                                                             StaticMeshKnife_->GetComponentLocation());

	FVector NewLocation = (ImpactLocation - (GetActorForwardVector() * DistanceBetweenMeshAndRootLocation));
	SetActorLocation(NewLocation + GetActorForwardVector() * DriveAttach);
}

void AKnife::CheckCollisionAttack()
{
	if (hasAlreadyAttack)
		return;

	FVector BoxPosition = GetActorLocation();
	FVector BoxExtent = BoxCollision->GetScaledBoxExtent() + OffsetBoxExtentCollisionAttack; 
	FQuat BoxRotation = GetActorQuat(); 

	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this); 
	Params.bTraceComplex = true; 

	FCollisionObjectQueryParams ObjectParams;
	ObjectParams.AddObjectTypesToQuery(ECC_GameTraceChannel4); 

	FHitResult OutHit; 

	bool bHit = GetWorld()->SweepSingleByObjectType(OutHit, BoxPosition, BoxPosition + FVector(0.f, 0.f, -1.f)
		, BoxRotation, ObjectParams,FCollisionShape::MakeBox(BoxExtent)
	);

	if (bHit)
		MakeDamage(OutHit);
	
	if (DebugCollisionAttack)
	{
		DrawDebugBox(GetWorld(),BoxPosition,BoxExtent,BoxRotation,FColor::Red,false, 5.0f );

		if (OutHit.GetActor() != nullptr)
		{
			GEngine->AddOnScreenDebugMessage(-1,10.0f,FColor::Red,"Hit : " + OutHit.GetActor()->GetName());
			UE_LOG(LogTemp, Warning, TEXT("Actor detected: %s"), *OutHit.GetActor()->GetName());
		}
	}
}

void AKnife::FinishCheckCollisionAttack()
{
	hasAlreadyAttack = false;
}

void AKnife::BeginPlay()
{
	Super::BeginPlay();
	BP_Chain = Cast<AChain>(Chain->GetChildActor());
}

void AKnife::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	FVector Velocity = ProjectileMovement->Velocity;
	FVector DeltaMove = ProjectileMovement->ComputeMoveDelta(Velocity, GetWorld()->GetDeltaSeconds());
	FVector HandLocation = 	HandStartLocation->GetComponentLocation() /*- DeltaMove*/;
	
	TArray<UStaticMeshComponent*> StaticMeshComponents = BP_Chain->GetStaticMeshComponents();
	int CountStaticMeshComponents = StaticMeshComponents.Num();
	
	if (CountStaticMeshComponents < 2)
		return;
	
	UStaticMeshComponent* LastStaticMeshComponent = StaticMeshComponents[CountStaticMeshComponents - 2];
	
	FVector LastChainLocation = LastStaticMeshComponent->GetComponentLocation();
	
	float LengthLinkChain = BP_Chain->GetLengthStaticMesh() * 2;
	float DistanceChainToHand = FVector::Distance(HandLocation,LastChainLocation);

	float NbMeshToAdd = DistanceChainToHand/LengthLinkChain;
	
	float CheckBehindHand = FVector::DotProduct(HandLocation - StaticMeshComponents[0]->GetComponentLocation(),
		HandLocation - LastChainLocation);

	FVector LineStart = HandStartLocation->GetComponentLocation();
	FVector LineEnd = HandLocation;
	
	DrawDebugDirectionalArrow(GetWorld(),LineStart,LineEnd,5,FColor::Red,true,-1,0,2);
	
	GEngine->AddOnScreenDebugMessage(-1,0,FColor::Red,"Check behind hand : " + FString::SanitizeFloat(CheckBehindHand));
	
	if (NbMeshToAdd >= 1)
	{
		for (int i = 0; i < NbMeshToAdd; i++)
		{
			if (CheckBehindHand >= 0)
			{
				GEngine->AddOnScreenDebugMessage(-1,5,FColor::Blue,"Velocity" + DeltaMove.ToString());
				// GEngine->AddOnScreenDebugMessage(-1,5,FColor::Red,"HandLocation" + HandStartLocation->GetComponentLocation().ToString());
				BP_Chain->AddDynamicMesh(IsAttached);
			}
		}
	}
}

void AKnife::MakeDamage(FHitResult OutHit)
{
	GAS_Utils::ApplyGameplayEffectToTargetSetByCaller(this,OutHit.GetActor(),GameplayEffectClass,PlayerAbilitySystemComponent,-Damage,FName("Event.Damage"));
	hasAlreadyAttack = true;
}
