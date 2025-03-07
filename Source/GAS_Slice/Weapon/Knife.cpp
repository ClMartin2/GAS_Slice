#include "Knife.h"
#include "Components/BoxComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/StaticMeshComponent.h"
#include "DrawDebugHelpers.h"
#include "Math/Quat.h"
#include "../Library/GAS_Utils.h"

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
}

void AKnife::Throw_Implementation(FVector DirectionThrowKnife, FVector NewCameraForward)
{
	IsAttached = false;
	ProjectileMovement->SetUpdatedComponent(GetRootComponent());
	ProjectileMovement->InitialSpeed = Speed;
	ProjectileMovement->Velocity = DirectionThrowKnife * Speed;
	CameraForward = NewCameraForward;
}

void AKnife::StopMove_Implementation()
{
	ProjectileMovement->Velocity = FVector::ZeroVector;
	ProjectileMovement->InitialSpeed = 0;
}

void AKnife::ResetKnife_Implementation()
{
	StopMove();
	IsAttached = false;
	BoxCollision->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
}

void AKnife::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
                   FVector NormalImpulse, const FHitResult& Hit)
{
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
	FVector BoxExtent = BoxCollision->GetScaledBoxExtent() + FVector::One() * OffsetBoxExtentCollisionAttack; 
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

void AKnife::MakeDamage(FHitResult OutHit)
{
	GAS_Utils::ApplyGameplayEffectToTargetSetByCaller(this,OutHit.GetActor(),GameplayEffectClass,PlayerAbilitySystemComponent,-Damage,FName("Event.Damage"));
	hasAlreadyAttack = true;
}
