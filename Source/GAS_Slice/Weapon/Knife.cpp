#include "Knife.h"

#include "Components/BoxComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/KismetStringLibrary.h"
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
	ProjectileMovement->SetUpdatedComponent(GetRootComponent());
	ProjectileMovement->InitialSpeed = Speed;
	ProjectileMovement->Velocity = DirectionThrowKnife * Speed;
	ThrowDirection = DirectionThrowKnife;
	CameraForward = NewCameraForward;
}

void AKnife::StopMove_Implementation()
{
	BoxCollision->SetSimulatePhysics(false);
	ProjectileMovement->Velocity = FVector::ZeroVector;
	ProjectileMovement->InitialSpeed = 0;
}

void AKnife::ResetKnife_Implementation()
{
	ProjectileMovement->Activate();
	BoxCollision->SetSimulatePhysics(false);
	StopMove();
}

void AKnife::Retain()
{
	StopMove();
	ProjectileMovement->Deactivate();
	BoxCollision->SetSimulatePhysics(true);
	BoxCollision->SetPhysicsLinearVelocity(FVector::ZeroVector);
	
	FVector ImpulseVector = ThrowDirection * RetainForce * -1;
	BoxCollision->AddImpulse(ImpulseVector);
}

void AKnife::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	StopMove();
	HitRotate(Hit);
	ReplaceHitKnife(Hit);
	AttachToActor(OtherActor, FAttachmentTransformRules::KeepWorldTransform);
	DelegateHitKnife.Broadcast();
}

void AKnife::RotateThrow()
{
	FQuat CurrentRotation = GetActorRotation().Quaternion();
	FRotator DeltaRotation = FRotator(-RotationSpeed * GetWorld()->GetDeltaSeconds(), 0, 0);
	FQuat NewRotation = CurrentRotation * DeltaRotation.Quaternion();
	
	SetActorRotation(NewRotation);
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
	FVector ImpactLocation = Hit.ImpactPoint;
	FRotator ActualRotation = GetActorRotation();
	float DistanceBetweenMeshAndRootLocation = FVector::Distance(GetActorLocation(), StaticMeshKnife_->GetComponentLocation());

	DrawDebugDirectionalArrow(GetWorld(),ImpactLocation, ImpactLocation + (StaticMeshKnife_->GetComponentLocation() - GetActorLocation()) * -1,
		10,FColor::Red,true);
	
	FVector NewLocation = GetActorLocation() - GetActorForwardVector() * DistanceBetweenMeshAndRootLocation;
	// SetActorLocation(NewLocation);
}

void AKnife::UpdateMove_Implementation()
{
	RotateThrow();
}
