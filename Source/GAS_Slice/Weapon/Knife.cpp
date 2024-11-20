// Fill out your copyright notice in the Description page of Project Settings.


#include "Knife.h"
#include "Components/BoxComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"


AKnife::AKnife()
{
	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));
	BoxCollision->OnComponentHit.AddDynamic(this, &AKnife::OnHit);

	SetRootComponent(BoxCollision);

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile Movement"));
	ProjectileMovement->UpdatedComponent = BoxCollision;
	ProjectileMovement->bRotationFollowsVelocity = true;
}

void AKnife::StartMove_Implementation(FVector DirectionThrowKnife)
{
	Direction = DirectionThrowKnife;
}

void AKnife::StopMove_Implementation()
{

}

void AKnife::Reset_Implementation()
{
	
}

void AKnife::Move(float Acceleration) {
	FVector NewLocation = GetActorLocation() + Direction * (SpeedKnife * Acceleration * GetWorld()->GetDeltaSeconds());
	SetActorLocation(NewLocation, true);
}

void AKnife::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	StopMove_Implementation();
	AttachToActor(OtherActor, FAttachmentTransformRules::KeepWorldTransform);
}
