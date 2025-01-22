#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Enemy.generated.h"

class USMC_Gun;
class UStaticMeshComponent;

UCLASS()
class GAS_SLICE_API AEnemy : public ACharacter
{
	GENERATED_BODY()

public:
	AEnemy();
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Mesh, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* BodyVisual;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Component, meta = (AllowPrivateAccess = "true"))
	USMC_Gun* SMCGun;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Component, meta = (AllowPrivateAccess = "true"))
	USceneComponent* BarrelPosition;
};
