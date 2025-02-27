// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../../Spells/ProjectileSpell/BaseProjectileSpell.h"
#include "GAS_Slice/Component/ActorComponent/AC_GameplayEffectSpell.h"
#include "GEProjectileSpell.generated.h"

/**
 * 
 */
UCLASS()
class GAS_SLICE_API AGEProjectileSpell : public ABaseProjectileSpell
{
	GENERATED_BODY()
public:
	AGEProjectileSpell();
	
protected:
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="ActorComponent", meta=(AllowPrivateAccess=true))
	UAC_GameplayEffectSpell* GameplayEffectSpell;

	UAbilitySystemComponent* GetOwnerAbilitySystemComponent() const {return OwnerAbilitySystemComponent;}
	
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Settings", meta=(AllowPrivateAccess=true))
	UAbilitySystemComponent* OwnerAbilitySystemComponent;
};
