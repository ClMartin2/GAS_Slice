// Fill out your copyright notice in the Description page of Project Settings.

#include "../../Spells/ZoneSpell/ZonePoison.h"

void AZonePoison::OnOverlapZone(const TArray<AActor*> OverlappingActors) const
{
	Super::OnOverlapZone(OverlappingActors);
	
	for (AActor* OverlappingActor : OverlappingActors)
	{
		GameplayEffectSpell->ApplyGameplayEffect(OverlappingActor, GetOwnerAbilitySystemComponent());
	}
}
