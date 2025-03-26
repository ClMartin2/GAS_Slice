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

	if (IsAttached)
	{
		CheckMeshToAdd = false;
		BP_Chain->DestroyAllDynamicMeshes();
	}
	
	AngularBreakable = false;
	StopMove();
	IsAttached = false;
	BoxCollision->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	GetWorldTimerManager().ClearTimer(TimerHandleSetPhysicsHit);
	GetWorldTimerManager().ClearTimer(TimerHandleSetAngularBreakable);
	BP_Chain->SetAngularBreakable(false);

	GetWorldTimerManager().SetTimer(TimerHandleCheckMeshToAdd, this, &AKnife::ResetCheckMeshToAdd, 1, false);
}

void AKnife::ResetCheckMeshToAdd()
{
	GetWorldTimerManager().ClearTimer(TimerHandleCheckMeshToAdd);
	CheckMeshToAdd = true;
}

void AKnife::StopMove_Implementation()
{
	ProjectileMovement->Velocity = FVector::ZeroVector;
	ProjectileMovement->InitialSpeed = 0;
}

void AKnife::SetChainPhysicsHit()
{
	BP_Chain->SetSimulatePhysics(true);
	GetWorldTimerManager().SetTimer(TimerHandleSetAngularBreakable, this, &AKnife::SetAngularBreakable, DelaySetAngularBreakable, false);
}

void AKnife::SetAngularBreakable()
{
	BP_Chain->SetAngularBreakable(true);
	AngularBreakable = true;
}

void AKnife::OnHit_Implementation(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
                                  FVector NormalImpulse, const FHitResult& Hit)
{
	GetWorldTimerManager().SetTimer(TimerHandleSetPhysicsHit, this, &AKnife::SetChainPhysicsHit, DelaySetPhysicsChain, false);
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
	BP_Chain->OnChainBreakDelegate.BindUObject(this, &AKnife::BreakChain);
}

void AKnife::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (!CheckMeshToAdd)
		return;
	
	FVector HandLocation = 	HandStartLocation->GetComponentLocation();
	
	TArray<UStaticMeshComponent*> StaticMeshComponents = BP_Chain->GetStaticMeshComponents();
	int CountStaticMeshComponents = StaticMeshComponents.Num();
	
	if (CountStaticMeshComponents < 2)
		return;
	
	UStaticMeshComponent* FirstStaticMeshComponent = StaticMeshComponents[0];
	
	FVector FirstChainLocation = FirstStaticMeshComponent->GetComponentLocation();
	
	float LengthLinkChain = (BP_Chain->GetLengthMesh() * StaticMeshKnife_->GetComponentScale()).X /*- BP_Chain->GetOffsetBetweenMesh()*/;
	float DistanceChainToHand = FVector::Distance(HandLocation,FirstChainLocation);

	float NbLinkOnChain = FMath::Floor((DistanceChainToHand/LengthLinkChain));

	int DifferenceBetweenChain = StaticMeshComponents.Num() - NbLinkOnChain;

	if (NbLinkOnChain >= 1)
	{
		if (DifferenceBetweenChain < 0)
		{
			for (int i = 0; i < DifferenceBetweenChain * -1; i++)
			{
				BP_Chain->AddDynamicMesh(IsAttached,AngularBreakable);
			}
		}else
		{
			int LengthArrayStaticMeshComponents = StaticMeshComponents.Num();
			
			for (int i = LengthArrayStaticMeshComponents - 1; i > LengthArrayStaticMeshComponents - DifferenceBetweenChain; i--)
			{
				BP_Chain->DestroyDynamicMesh(StaticMeshComponents[i]);
			}
		}
	}
}

void AKnife::MakeDamage(FHitResult OutHit)
{
	GAS_Utils::ApplyGameplayEffectToTargetSetByCaller(this,OutHit.GetActor(),GameplayEffectClass,PlayerAbilitySystemComponent,-Damage,FName("Event.Damage"));
	hasAlreadyAttack = true;
}

void AKnife::BreakChain()
{
	OnChainBreak.Execute();
}
