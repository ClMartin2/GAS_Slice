// Fill out your copyright notice in the Description page of Project Settings.


#include "../Spells/ShieldSpell.h"
#include "GAS_Slice/Component/ActorComponent/AC_GameplayEffectSpell.h"
#include "GAS_Slice/Library/GAS_Utils.h"

AShieldSpell::AShieldSpell()
{
	GameplayEffectSpell = CreateDefaultSubobject<UAC_GameplayEffectSpell>(TEXT("ComponentGameplayEffectSpell"));
}

void AShieldSpell::BeginPlay()
{
	Super::BeginPlay();
	OwnerAbilitySystemComponent = GAS_Utils::GetAbilitySystem(GetOwner());
	GameplayEffectSpell->ApplyGameplayEffectToSelf(GetOwnerAbilitySystemComponent());
}
