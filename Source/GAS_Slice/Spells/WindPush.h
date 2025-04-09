// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../Spells/BaseSpell.h"
#include "WindPush.generated.h"

/**
 * 
 */
UCLASS()
class GAS_SLICE_API AWindPush : public ABaseSpell
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Settings", meta=(allowPrivateAccess="true"))
	float PushForce = 2000;
	
protected:
	virtual void BeginPlay() override;
};
