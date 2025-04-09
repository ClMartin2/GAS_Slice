// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../Spells/BaseSpell.h"
#include "ShieldSpell.generated.h"

class UAbilitySystemComponent;
class UAC_GameplayEffectSpell;

UCLASS()
class GAS_SLICE_API AShieldSpell : public ABaseSpell
{
	GENERATED_BODY()

public:
	AShieldSpell();
	
protected:
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="ActorComponent", meta=(AllowPrivateAccess=true))
	UAC_GameplayEffectSpell* GameplayEffectSpell;

private:
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Settings", meta=(AllowPrivateAccess=true))
	UAbilitySystemComponent* OwnerAbilitySystemComponent;

protected:
	virtual void BeginPlay() override;
	
	UAbilitySystemComponent* GetOwnerAbilitySystemComponent() const {return OwnerAbilitySystemComponent;}

};
