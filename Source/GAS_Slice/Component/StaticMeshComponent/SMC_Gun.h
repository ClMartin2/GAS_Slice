#pragma once

#include "CoreMinimal.h"
#include "Components/StaticMeshComponent.h"
#include "SMC_Gun.generated.h"

class UAbilitySystemComponent;

UCLASS()
class GAS_SLICE_API USMC_Gun : public UStaticMeshComponent
{
	GENERATED_BODY()
public:
	USMC_Gun();

private:
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Settings", meta=(AllowPrivateAccess=true))
	USceneComponent* BarrelPostion;

	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Settings", meta=(AllowPrivateAccess=true))
	TSubclassOf<AActor> ClassBullet;

	UAbilitySystemComponent* EnemyAbilitySystemComponent;

public:
	void SetBarrelPostion(USceneComponent* NewBarrelPostion);
	void SetAbilitySystemComponent(UAbilitySystemComponent* AbilitySystemComponent){ EnemyAbilitySystemComponent = AbilitySystemComponent;};
	
private:
	UFUNCTION(BlueprintCallable,Category="Shoot",meta = (AllowPrivateAccess = "true"))
	void Shoot();
};
