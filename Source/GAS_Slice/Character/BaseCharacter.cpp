#include "../Character/BaseCharacter.h"
#include "AbilitySystemComponent.h"
#include "../GAS/AttributeSet/BasicAttributeSet.h"


ABaseCharacter::ABaseCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbiliySystemComponent"));
}

void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	if (IsValid(AbilitySystemComponent))
	{
		BasicAttributeset = AbilitySystemComponent->GetSet<UBasicAttributeSet>();	
	}
}

