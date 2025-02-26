// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "GameplayEffectTypes.h"
#include "BaseCharacter.generated.h"

class UAbilitySystemComponent;
class UBasicAttributeSet;

UCLASS()
class GAS_SLICE_API ABaseCharacter : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	ABaseCharacter();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="GAS", meta=(AllowPrivateAccess="true"))
	const class UBasicAttributeSet* BasicAttributeset;
	
protected:	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="GAS", meta=(AllowPrivateAccess="true"))
	class UAbilitySystemComponent* AbilitySystemComponent;

	virtual UAbilitySystemComponent* GetAbilitySystemComponent()const override{return AbilitySystemComponent;}
	
protected:
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintImplementableEvent, Category=Events)
	void OnBeginPlay();

	UFUNCTION(BlueprintNativeEvent)
	void Death();

	virtual void Death_Implementation();
	virtual void OnHealthChanged(const FOnAttributeChangeData& Data);
};
