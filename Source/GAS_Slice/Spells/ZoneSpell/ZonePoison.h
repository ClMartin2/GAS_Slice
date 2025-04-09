// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../../Spells/ZoneSpell/GEZoneSpell.h"
#include "ZonePoison.generated.h"

/**
 * 
 */
UCLASS()
class GAS_SLICE_API AZonePoison : public AGEZoneSpell
{
	GENERATED_BODY()

protected:
	virtual void OnOverlapZone(const TArray<AActor*> OverlappingActors) const override;
};
