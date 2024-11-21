// Fill out your copyright notice in the Description page of Project Settings.


#include "Knife.h"
#include "Components/BoxComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"


AKnife::AKnife()
{
	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));
	BoxCollision->OnComponentHit.AddDynamic(this, &AKnife::OnHit);

	RootComponent = BoxCollision;

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile Movement"));
	ProjectileMovement->UpdatedComponent = BoxCollision;
	ProjectileMovement->bRotationFollowsVelocity = true;

	ProjectileMovement->MaxSpeed = MaxSpeed;
}

void AKnife::StartMove_Implementation(FVector DirectionThrowKnife)
{
	Direction = DirectionThrowKnife;
	ProjectileMovement->InitialSpeed = Speed;
	ProjectileMovement->Velocity = Direction * Speed;

	GEngine->AddOnScreenDebugMessage(-1, 500, FColor::Blue, "Velocity: " + ProjectileMovement->Velocity.ToString());
}

void AKnife::StopMove_Implementation()
{
	ProjectileMovement->Velocity = FVector::ZeroVector;
	ProjectileMovement->InitialSpeed = 0;
}

void AKnife::Reset_Implementation()
{
	StopMove_Implementation();
}

void AKnife::Move(float Acceleration) {
	FVector NewLocation = GetActorLocation() + Direction * (Speed * Acceleration * GetWorld()->GetDeltaSeconds());
	SetActorLocation(NewLocation, true);
}

void AKnife::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	StopMove_Implementation();
	AttachToActor(OtherActor, FAttachmentTransformRules::KeepWorldTransform);
}
