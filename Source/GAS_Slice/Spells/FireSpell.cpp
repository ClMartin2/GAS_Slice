#include "../Spells/FireSpell.h"

void AFireSpell::OnOverlapActor(AActor* CollideActor)
{
	Super::OnOverlapActor(CollideActor);
	GAS_Utils::ApplyGameplayEffectToTargetSetByCaller(this,CollideActor,GameplayEffectClass,OwnerAbilitySystemComponent,SpellValue
		,EffectGameplayTagName,DurationSpell);
}
