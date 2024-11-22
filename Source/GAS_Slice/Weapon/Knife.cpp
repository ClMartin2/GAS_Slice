// Fill out your copyright notice in the Description page of Project Settings.


#include "Knife.h"
#include "Components/BoxComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/KismetStringLibrary.h"
#include "../Library/ConvertLibrary.h"


AKnife::AKnife()
{
	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));
	BoxCollision->OnComponentHit.AddDynamic(this, &AKnife::OnHit);

	RootComponent = BoxCollision;

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile Movement"));
	ProjectileMovement->UpdatedComponent = BoxCollision;
	ProjectileMovement->bRotationFollowsVelocity = true;

	ProjectileMovement->MaxSpeed = MaxSpeed;

	StaticMeshKnife = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshKnife"));
	StaticMeshKnife->SetupAttachment(BoxCollision);
}

void AKnife::StartMove_Implementation(FVector DirectionThrowKnife)
{
	ProjectileMovement->SetUpdatedComponent(GetRootComponent());
	ProjectileMovement->InitialSpeed = Speed;
	ProjectileMovement->Velocity = DirectionThrowKnife * Speed;
}

void AKnife::StopMove_Implementation()
{
	ProjectileMovement->Velocity = FVector::ZeroVector;
	ProjectileMovement->InitialSpeed = 0;
}

void AKnife::Reset_Implementation()
{
	StopMove();
}

void AKnife::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	HitKnife(OtherActor, Hit);
}

void AKnife::RotateThrow()
{
	FRotator NewRotation = GetActorRotation() - FRotator(RotationSpeed * GetWorld()->GetDeltaSeconds(),0, 0);
	SetActorRotation(NewRotation);
}

void AKnife::HitKnife_Implementation(AActor* HitActor, FHitResult Hit)
{
	FVector Forward = GetActorForwardVector();
	bool IsOnTheRightSide = !ConvertLibrary::ConvertFloatToBoolNegativePositiveRange(FVector::DotProduct(Forward, Hit.ImpactNormal));

	if (!IsOnTheRightSide) {
		FRotator ActorRotation = GetActorRotation();
		FRotator NewRotation = FRotator(ActorRotation.Pitch, ActorRotation.Yaw + 180.0f, ActorRotation.Roll);
		SetActorRotation(NewRotation);
		GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Black, "Inverse");
	}

	StopMove();
	AttachToActor(HitActor, FAttachmentTransformRules::KeepWorldTransform);
}
