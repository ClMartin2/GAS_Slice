#include "../../Spells/ProjectileSpell/GEProjectileSpell.h"
#include "GAS_Slice/Library/GAS_Utils.h"

AGEProjectileSpell::AGEProjectileSpell()
{
	GameplayEffectSpell = CreateDefaultSubobject<UAC_GameplayEffectSpell>(TEXT("ComponentGameplayEffectSpell"));
}

void AGEProjectileSpell::BeginPlay()
{
	Super::BeginPlay();
	OwnerAbilitySystemComponent = GAS_Utils::GetAbilitySystem(GetOwner());
}


