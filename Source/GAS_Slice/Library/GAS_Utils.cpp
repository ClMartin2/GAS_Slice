#include "GAS_Utils.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "GameplayEffect.h"

void GAS_Utils::ApplyGameplayEffectToTargetSetByCaller(UObject* Source,AActor* TargetActor, TSubclassOf<UGameplayEffect> GameplayEffectClass 
	, UAbilitySystemComponent* AbilitySystemComponent, float Value, FName GameplayTagName,float Duration, float Level)
{
	UAbilitySystemComponent* TargetAbilitySystemComponent = GetAbilitySystem(TargetActor);
	
	if (TargetAbilitySystemComponent!= nullptr)
	{
		FGameplayEffectContextHandle EffectContext = TargetAbilitySystemComponent->MakeEffectContext();
		EffectContext.AddSourceObject(Source);

		FGameplayEffectSpecHandle EffectSpecHandle = TargetAbilitySystemComponent->MakeOutgoingSpec(GameplayEffectClass, Level, EffectContext);
		
		if (EffectSpecHandle.IsValid())
		{
			FGameplayTag GameplayTag = FGameplayTag::RequestGameplayTag(GameplayTagName);
			EffectSpecHandle.Data->SetSetByCallerMagnitude(GameplayTag, Value);
			EffectSpecHandle.Data->SetDuration(Duration,true);
			AbilitySystemComponent->ApplyGameplayEffectSpecToTarget(*EffectSpecHandle.Data.Get(), TargetAbilitySystemComponent);
		}
	}
}

UAbilitySystemComponent* GAS_Utils::GetAbilitySystem(AActor* TargetActor)
{
	if (IAbilitySystemInterface* ASCInterface = Cast<IAbilitySystemInterface>(TargetActor))
	{
		return ASCInterface->GetAbilitySystemComponent();
	}
	
	return nullptr;
}
