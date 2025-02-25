// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GEProjectileSpell.h"
#include "FireSpell.generated.h"


UCLASS()
class GAS_SLICE_API AFireSpell : public AGEProjectileSpell
{
	GENERATED_BODY()
protected:
	virtual void OnOverlapActor(AActor* CollideActor) override;
};
