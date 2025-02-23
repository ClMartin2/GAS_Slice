// Fill out your copyright notice in the Description page of Project Settings.


#include "Utils.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "GameplayEffect.h"

Utils::Utils()
{
}

Utils::~Utils()
{
}

bool Utils::ConvertFloatToBoolNegativePositiveRange(float Value)
{
	return Value >= 0.0f;
}

void Utils::ApplyGameplayEffectToTargetSetByCaller(UObject* Source,AActor* TargetActor, TSubclassOf<UGameplayEffect> GameplayEffectClass 
	, UAbilitySystemComponent* AbilitySystemComponent, float Value, FName GameplayTagName,float Level)
{
	if (IAbilitySystemInterface* ASCInterface = Cast<IAbilitySystemInterface>(TargetActor))
	{
		UAbilitySystemComponent* TargetAbilitySystemComponent = ASCInterface->GetAbilitySystemComponent();
		FGameplayEffectContextHandle EffectContext = TargetAbilitySystemComponent->MakeEffectContext();
		EffectContext.AddSourceObject(Source);

		FGameplayEffectSpecHandle EffectSpecHandle = TargetAbilitySystemComponent->MakeOutgoingSpec(GameplayEffectClass, Level, EffectContext);
		
		if (EffectSpecHandle.IsValid())
		{
			EffectSpecHandle.Data->SetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag(GameplayTagName), Value);
			AbilitySystemComponent->ApplyGameplayEffectSpecToTarget(*EffectSpecHandle.Data.Get(), TargetAbilitySystemComponent);
		}
	}
}
