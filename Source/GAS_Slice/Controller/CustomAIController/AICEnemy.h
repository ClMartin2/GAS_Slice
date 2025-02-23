#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "GenericTeamAgentInterface.h"
#include "AICEnemy.generated.h"

/**
 * 
 */
UCLASS()
class GAS_SLICE_API AAICEnemy : public AAIController
{
	GENERATED_BODY()
	AAICEnemy();
private:
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Settings|ID",meta=(AllowPrivateAccess=true))
	uint8 TeamId = 0;

public:
	ETeamAttitude::Type GetTeamAttitudeTowards(const AActor& Other) const override;
	
private:
	virtual void BeginPlay() override;
};

