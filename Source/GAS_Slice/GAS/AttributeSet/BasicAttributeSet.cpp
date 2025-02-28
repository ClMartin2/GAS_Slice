// Fill out your copyright notice in the Description page of Project Settings.

#include "GameplayEffect.h"
#include "GameplayEffectExtension.h"
#include "../../GAS/AttributeSet/BasicAttributeSet.h"

void UBasicAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	if (Attribute == GetManaAttribute())
		NewValue = FMath::Clamp<float>(NewValue, 0.0f, GetMaxMana());
	if (Attribute == GetShieldAttribute())
		NewValue = FMath::Clamp<float>(NewValue, GetMinShield(), 99999999999);

	Super::PreAttributeChange(Attribute, NewValue);
}

void UBasicAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	if (Data.EvaluatedData.Attribute == GetManaAttribute())
		Mana.SetBaseValue(FMath::Clamp(Mana.GetCurrentValue(), 0.0f,  GetMaxMana()));

	Super::PostGameplayEffectExecute(Data);
}
