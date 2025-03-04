// Fill out your copyright notice in the Description page of Project Settings.

#include "Kismet/KismetSystemLibrary.h"
#include "DrawDebugHelpers.h"
#include "../../Spells/ZoneSpell/GEZoneSpell.h"
#include "Curves/CurveFloat.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

void AGEZoneSpell::OnOverlapActor(AActor* CollideActor)
{
	Super::OnOverlapActor(CollideActor);
	SphereCollider->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	ProjectileMovement->Velocity = FVector::ZeroVector;
	ProjectileMovement->ProjectileGravityScale = 0;
	Mesh->SetHiddenInGame(true);
	Mesh->SetVisibility(false);
	
	GetWorldTimerManager().SetTimer(UpdateZoneCollisionTimerHandle, this, &AGEZoneSpell::ZoneCollisionUpdate,
		ZoneTick, true,0);
	GetWorldTimerManager().SetTimer(FinishedZoneCollisionTimerHandle, this, &AGEZoneSpell::ZoneCollisionFinished,
	ZoneDuration, false);
}

void AGEZoneSpell::OnOverlapZone(const TArray<AActor*> OverlappingActors) const
{
}

void AGEZoneSpell::ZoneCollisionUpdate() const
{
	FVector SphereCenter = GetActorLocation();
	
	TArray<AActor*> OverlappingActors;
	TArray<AActor*> IgnoredActors;
    
	ECollisionChannel CollisionChannel = ECC_GameTraceChannel4;

	bool bHit = UKismetSystemLibrary::SphereOverlapActors(
		GetWorld(), 
		SphereCenter, 
		ZoneRadius, 
		{ UEngineTypes::ConvertToObjectType(CollisionChannel) }, 
		nullptr, 
		IgnoredActors, 
		OverlappingActors
	);

	if (Debug)
		DrawDebugSphere(GetWorld(), SphereCenter, ZoneRadius, 16, FColor::Red, false, ZoneDuration);

	if (bHit)
		OnOverlapZone(OverlappingActors);
}

void AGEZoneSpell::ZoneCollisionFinished()
{
	GetWorldTimerManager().ClearTimer(UpdateZoneCollisionTimerHandle);
	Death();
}

void AGEZoneSpell::Death_Implementation()
{
	Super::Death_Implementation();
	Destroy();
}