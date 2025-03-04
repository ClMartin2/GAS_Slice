// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseSpell.generated.h"

UCLASS()
class GAS_SLICE_API ABaseSpell : public AActor
{
	GENERATED_BODY()
	
public:	
	ABaseSpell() = default;

protected:
	UFUNCTION(BlueprintNativeEvent,meta=(AllowPrivateAccess=true))
	void Death();

	virtual void Death_Implementation();
};
