// Fill out your copyright notice in the Description page of Project Settings.

#include "../Spells/GameplayEffectSpell.h"

void AGameplayEffectSpell::BeginPlay()
{
	Super::BeginPlay();
	OwnerAbilitySystemComponent = GAS_Utils::GetAbilitySystem(GetOwner());
}
