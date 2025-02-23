// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffect.h"

/**
 * 
 */
class GAS_SLICE_API Utils
{
public:
	Utils();
	~Utils();

	static bool ConvertFloatToBoolNegativePositiveRange(float Value);
	static void ApplyGameplayEffectToTargetSetByCaller(UObject* Source, AActor* TargetActor, TSubclassOf<UGameplayEffect> GameplayEffectClass, UAbilitySystemComponent
	                                                   * AbilitySystemComponent, float Value, FName GameplayTagName,float Level = 0);
};
