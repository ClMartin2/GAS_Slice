#pragma once

#include "CoreMinimal.h"
#include "../Spells/BaseSpell.h"
#include "AbilitySystemComponent.h"
#include "GAS_Slice/Library/GAS_Utils.h"
#include "GameplayEffectSpell.generated.h"

UCLASS()
class GAS_SLICE_API AGameplayEffectSpell : public ABaseSpell
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Settings", meta=(AllowPrivateAccess=true))
	TSubclassOf<UGameplayEffect> GameplayEffectClass;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Settings", meta=(AllowPrivateAccess=true))
	UAbilitySystemComponent* OwnerAbilitySystemComponent;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Settings", meta=(AllowPrivateAccess=true))
	float SpellValue;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Settings", meta=(AllowPrivateAccess=true))
	FName EffectGameplayTagName = "Event.Spell" ;

protected:
	virtual void BeginPlay() override;
};
