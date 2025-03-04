// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../../Spells/ProjectileSpell/GEProjectileSpell.h"
#include "GEZoneSpell.generated.h"

class UCurveFloat;

/**
 * 
 */
UCLASS()
class GAS_SLICE_API AGEZoneSpell : public AGEProjectileSpell
{
	GENERATED_BODY()
private:
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Settings",meta=(AllowPrivateAccess=true))
	float ZoneDuration = 1;

	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Settings",meta=(AllowPrivateAccess=true))
	float ZoneRadius = 500;

	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Settings",meta=(AllowPrivateAccess=true))
	float ZoneTick = 0.1;

	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Settings|Debug",meta=(AllowPrivateAccess=true))
	bool Debug = false;

	float TickCounter = 0;
	
protected:
	virtual void OnOverlapActor(AActor* CollideActor) override;
	virtual void OnOverlapZone(const TArray<AActor*> OverlappingActors) const;
	virtual void Death_Implementation() override;

private:
	FTimerHandle UpdateZoneCollisionTimerHandle;
	FTimerHandle FinishedZoneCollisionTimerHandle;

	UFUNCTION()
	void ZoneCollisionUpdate() const;

	UFUNCTION()
	void ZoneCollisionFinished();

};
