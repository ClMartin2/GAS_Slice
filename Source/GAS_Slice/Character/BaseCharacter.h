// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "BaseCharacter.generated.h"

class UAbilitySystemComponent;

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

};
