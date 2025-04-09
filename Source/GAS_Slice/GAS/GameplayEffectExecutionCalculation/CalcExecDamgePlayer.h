// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectExecutionCalculation.h"
#include "CalcExecDamgePlayer.generated.h"

/**
 * 
 */
UCLASS()
class GAS_SLICE_API UCalcExecDamgePlayer : public UGameplayEffectExecutionCalculation
{
	GENERATED_BODY()
	
	UCalcExecDamgePlayer();
	virtual void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
		FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;
};
