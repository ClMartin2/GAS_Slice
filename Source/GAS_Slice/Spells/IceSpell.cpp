// Fill out your copyright notice in the Description page of Project Settings.


#include "../Spells/IceSpell.h"

void AIceSpell::OnOverlapActor(AActor* CollideActor)
{
	Super::OnOverlapActor(CollideActor);
	GameplayEffectSpell->ApplyGameplayEffect(CollideActor, GetOwnerAbilitySystemComponent());
}