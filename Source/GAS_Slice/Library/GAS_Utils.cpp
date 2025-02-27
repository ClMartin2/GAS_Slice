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
		AbilitySystemComponent->ApplyGameplayEffectSpecToTarget(*MakeEffectSpecHandle(Source,GameplayEffectClass,TargetAbilitySystemComponent,
				Value,GameplayTagName,Duration,Level).Data.Get(), TargetAbilitySystemComponent);
	}
}

void GAS_Utils::ApplyGameplayEffectToSelfSetByCaller(UObject* Source, TSubclassOf<UGameplayEffect> GameplayEffectClass,
	UAbilitySystemComponent* AbilitySystemComponent, float Value, FName GameplayTagName, float Duration, float Level)
{
	AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*MakeEffectSpecHandle(Source,GameplayEffectClass,AbilitySystemComponent,
		Value,GameplayTagName,Duration,Level).Data);
}

UAbilitySystemComponent* GAS_Utils::GetAbilitySystem(AActor* TargetActor)
{
	if (IAbilitySystemInterface* ASCInterface = Cast<IAbilitySystemInterface>(TargetActor))
	{
		return ASCInterface->GetAbilitySystemComponent();
	}
	
	return nullptr;
}

FGameplayEffectSpecHandle GAS_Utils::MakeEffectSpecHandle(UObject* Source, TSubclassOf<UGameplayEffect> GameplayEffectClass,
	UAbilitySystemComponent* AbilitySystemComponent, float Value, FName GameplayTagName, float Duration, float Level)
{
	FGameplayEffectContextHandle EffectContext = AbilitySystemComponent->MakeEffectContext();
	EffectContext.AddSourceObject(Source);

	FGameplayEffectSpecHandle EffectSpecHandle = AbilitySystemComponent->MakeOutgoingSpec(GameplayEffectClass, Level, EffectContext);
		
	if (EffectSpecHandle.IsValid())
	{
		FGameplayTag GameplayTag = FGameplayTag::RequestGameplayTag(GameplayTagName);
		EffectSpecHandle.Data->SetSetByCallerMagnitude(GameplayTag, Value);
		EffectSpecHandle.Data->SetDuration(Duration,true);
	}
	
	return EffectSpecHandle;
}
