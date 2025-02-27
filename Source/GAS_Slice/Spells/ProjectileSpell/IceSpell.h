// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../../Spells/ProjectileSpell/GEProjectileSpell.h"
#include "IceSpell.generated.h"

/**
 * 
 */
UCLASS()
class GAS_SLICE_API AIceSpell : public AGEProjectileSpell
{
protected:
	GENERATED_BODY()
	virtual void OnOverlapActor(AActor* CollideActor) override;
};
