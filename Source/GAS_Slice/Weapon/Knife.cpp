#include "Knife.h"
#include "Components/BoxComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/StaticMeshComponent.h"
#include "DrawDebugHelpers.h"
#include "Math/Quat.h"

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

void AKnife::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	GEngine->AddOnScreenDebugMessage(-1,100,FColor::Green,"On hit Knife");
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

	FRotator NewRotation = FRotator(CameraForward.Rotation().Pitch,CameraForward.Rotation().Yaw,HitNormalRotation.Roll);
	
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
	
	GetWorld()->LineTraceSingleByChannel(HitResult, Start, End,ECollisionChannel::ECC_Visibility, RV_TraceParams);
	
	FVector ImpactLocation = HitResult.ImpactPoint;
	float DistanceBetweenMeshAndRootLocation = FVector::Distance(GetActorLocation(), StaticMeshKnife_->GetComponentLocation());
	
	FVector NewLocation = (ImpactLocation - (GetActorForwardVector() * DistanceBetweenMeshAndRootLocation));
	SetActorLocation(NewLocation + GetActorForwardVector() * DriveAttach);
}
