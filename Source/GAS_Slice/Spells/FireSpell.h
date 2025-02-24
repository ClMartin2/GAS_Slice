// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../Spells/BaseProjectileSpell.h"
#include "FireSpell.generated.h"


UCLASS()
class GAS_SLICE_API AFireSpell : public ABaseProjectileSpell
{
	GENERATED_BODY()
protected:
	virtual void OnOverlapActor(AActor* CollideActor) override;
};
