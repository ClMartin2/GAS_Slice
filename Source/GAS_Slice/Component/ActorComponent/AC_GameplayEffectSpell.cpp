// Fill out your copyright notice in the Description page of Project Settings.


#include "../../Component/ActorComponent/AC_GameplayEffectSpell.h"
#include "GAS_Slice/Library/GAS_Utils.h"

void UAC_GameplayEffectSpell::ApplyGameplayEffect(AActor* TargetActor, UAbilitySystemComponent* AbilitySystemComponent)
{
	GAS_Utils::ApplyGameplayEffectToTargetSetByCaller(this,TargetActor,GameplayEffectClass,AbilitySystemComponent,SpellValue
		,EffectGameplayTagName,DurationSpell);
}


