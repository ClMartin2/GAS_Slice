#include "../../Spells/ProjectileSpell/FireSpell.h"

void AFireSpell::OnOverlapActor(AActor* CollideActor)
{
	Super::OnOverlapActor(CollideActor);
	GameplayEffectSpell->ApplyGameplayEffect(CollideActor, GetOwnerAbilitySystemComponent());
}
