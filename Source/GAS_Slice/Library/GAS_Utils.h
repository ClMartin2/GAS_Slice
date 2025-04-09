#pragma once

#include "CoreMinimal.h"

class UAbilitySystemComponent;
class UGameplayEffect;
struct FGameplayEffectSpecHandle;

namespace GAS_Utils
{
	void ApplyGameplayEffectToTargetSetByCaller(UObject* Source, AActor* TargetActor, TSubclassOf<UGameplayEffect> GameplayEffectClass,
		UAbilitySystemComponent* AbilitySystemComponent, float Value, FName GameplayTagName,float Duration = 0,float Level = 0);

	void ApplyGameplayEffectToSelfSetByCaller(UObject* Source, TSubclassOf<UGameplayEffect> GameplayEffectClass,
		UAbilitySystemComponent* AbilitySystemComponent, float Value, FName GameplayTagName,float Duration = 0,float Level = 0);

	UAbilitySystemComponent* GetAbilitySystem(AActor* TargetActor);

	FGameplayEffectSpecHandle MakeEffectSpecHandle(UObject* Source, TSubclassOf<UGameplayEffect> GameplayEffectClass,
	UAbilitySystemComponent* AbilitySystemComponent, float Value, FName GameplayTagName, float Duration, float Level);
}
