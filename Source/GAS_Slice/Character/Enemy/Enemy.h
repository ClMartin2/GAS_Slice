#pragma once

#include "CoreMinimal.h"
#include "GAS_Slice/Character/BaseCharacter.h"
#include "Enemy.generated.h"

class UAC_Health;
class USMC_Gun;
class USkeletalMeshComponent;

UCLASS()
class GAS_SLICE_API AEnemy : public ABaseCharacter
{
	GENERATED_BODY()

public:
	AEnemy();
	
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly, Category="Visual")
	USkeletalMeshComponent* BodyVisual;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Component)
	USMC_Gun* SMCGun;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Component)
	USceneComponent* BarrelPosition;

protected:
	virtual void BeginPlay() override;
	
private:
	virtual void Death_Implementation() override;
};
