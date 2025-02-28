// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GAS_SliceProjectile.h"
#include "EnemyProjectile.generated.h"

class UAbilitySystemComponent;
class UGameplayEffect;
/**
 * 
 */
UCLASS()
class GAS_SLICE_API AEnemyProjectile : public AGAS_SliceProjectile
{
	GENERATED_BODY()
	
private:
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Settings",meta=(AllowPrivateAccess=true))
	float Damage;
	
	UAbilitySystemComponent* EnemyAbilitySystemComponent;
	
	UPROPERTY(EditAnywhere, Category = "Settings|GAS", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UGameplayEffect> GameplayEffectClassHealth;

	UPROPERTY(EditAnywhere, Category = "Settings|GAS", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UGameplayEffect> GameplayEffectClassShield;
public:
	void SetAbilitySystemComponent(UAbilitySystemComponent* AbilitySystemComponent){ EnemyAbilitySystemComponent = AbilitySystemComponent;};
	
protected:
	virtual void BeginPlay() override;
	
private:
	void Death();

	UFUNCTION()
	void OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
