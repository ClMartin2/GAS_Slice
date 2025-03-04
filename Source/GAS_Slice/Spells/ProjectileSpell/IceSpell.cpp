// Fill out your copyright notice in the Description page of Project Settings.


#include "../../Spells/ProjectileSpell/IceSpell.h"

void AIceSpell::OnOverlapActor(AActor* CollideActor)
{
	Super::OnOverlapActor(CollideActor);
	GameplayEffectSpell->ApplyGameplayEffectToTarget(CollideActor, GetOwnerAbilitySystemComponent());
	Death();
}

void AIceSpell::Death_Implementation()
{
	Super::Death_Implementation();
	Destroy();
}
