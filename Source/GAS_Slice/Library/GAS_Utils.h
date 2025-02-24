#pragma once

#include "CoreMinimal.h"

class UAbilitySystemComponent;
class UGameplayEffect;

namespace GAS_Utils
{
	void ApplyGameplayEffectToTargetSetByCaller(UObject* Source, AActor* TargetActor, TSubclassOf<UGameplayEffect> GameplayEffectClass,
		UAbilitySystemComponent* AbilitySystemComponent, float Value, FName GameplayTagName,float Level = 0);

	UAbilitySystemComponent* GetAbilitySystem(AActor* TargetActor);
}
