#include "../Character/BaseCharacter.h"
#include "AbilitySystemComponent.h"


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

