// Fill out your copyright notice in the Description page of Project Settings.


#include "Knife.h"

#include <string>

#include "Components/BoxComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/KismetStringLibrary.h"
#include "../Library/ConvertLibrary.h"
#include "DrawDebugHelpers.h"

AKnife::AKnife()
{
	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));
	BoxCollision->OnComponentHit.AddDynamic(this, &AKnife::OnHit);

	RootComponent = BoxCollision;

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile Movement"));
	ProjectileMovement->UpdatedComponent = BoxCollision;
	ProjectileMovement->bRotationFollowsVelocity = true;

	ProjectileMovement->MaxSpeed = MaxSpeed;

	StaticMeshKnife_ = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh Knife"));
	StaticMeshKnife_->SetupAttachment(BoxCollision);
}

void AKnife::StartMove_Implementation(FVector DirectionThrowKnife)
{
	ProjectileMovement->SetUpdatedComponent(GetRootComponent());
	ProjectileMovement->InitialSpeed = Speed;
	ProjectileMovement->Velocity = DirectionThrowKnife * Speed;
	ThrowDirection = DirectionThrowKnife;
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

	GEngine->AddOnScreenDebugMessage(-1,3,FColor::Black,"Is On The Right Side: "
		+ UKismetStringLibrary::Conv_BoolToString(IsOnTheRightSide));
	
	if (!IsOnTheRightSide) {
		FRotator ActorRotation = GetActorRotation();
		FRotator NewRotation = FRotator(ActorRotation.Pitch, ActorRotation.Yaw + 180.0f, ActorRotation.Roll);
		SetActorRotation(NewRotation);
	}

	StopMove();
	AttachToActor(HitActor, FAttachmentTransformRules::KeepWorldTransform);

	DelegateHitKnife.Broadcast();
}

void AKnife::UpdateMove_Implementation()
{
	RotateThrow();
}
