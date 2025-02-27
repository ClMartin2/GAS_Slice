// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AbilitySystemComponent.h"
#include "AC_GameplayEffectSpell.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GAS_SLICE_API UAC_GameplayEffectSpell : public UActorComponent
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Settings", meta=(AllowPrivateAccess=true))
	TSubclassOf<UGameplayEffect> GameplayEffectClass;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Settings", meta=(AllowPrivateAccess=true))
	float SpellValue;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Settings", meta=(AllowPrivateAccess=true))
	float DurationSpell;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Settings", meta=(AllowPrivateAccess=true))
	FName EffectGameplayTagName = "Event.Spell";

public:
	void ApplyGameplayEffectToTarget(AActor* TargetActor, UAbilitySystemComponent* AbilitySystemComponent);
	void ApplyGameplayEffectToSelf(UAbilitySystemComponent* AbilitySystemComponent);
};
