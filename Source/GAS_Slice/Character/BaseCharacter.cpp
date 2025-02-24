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
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(UBasicAttributeSet::GetHealthAttribute()).AddUObject(this, &ABaseCharacter::OnHealthChanged);
	}
}

void ABaseCharacter::Death_Implementation()
{
}

void ABaseCharacter::OnHealthChanged(const FOnAttributeChangeData& Data)
{
	float NewHealth = Data.NewValue;
	
	if (NewHealth <= 0)
	{
		Death();
	}
}

