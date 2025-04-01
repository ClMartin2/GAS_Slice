// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnemySpawner.generated.h"

class AEnemy;

UCLASS()

class GAS_SLICE_API AEnemySpawner : public AActor
{
	GENERATED_BODY()

public :
	AEnemySpawner();

private:
	UPROPERTY(EditAnywhere,Category="Setttings", meta=(AllowPrivateAccess=true))
	int NumberEnemyToSpawnAtStart = 10;

	UPROPERTY(EditAnywhere,Category="Setttings", meta=(AllowPrivateAccess=true))
	int NumberEnemyToSpawn = 10;

	UPROPERTY(EditAnywhere,Category="Setttings", meta=(AllowPrivateAccess=true))
	float SpawnEnemyToXSeconds = 1;

	UPROPERTY(EditAnywhere,Category="Setttings", meta=(AllowPrivateAccess=true))
	float RadiusEnemyToSpawn = 1;

	UPROPERTY(EditAnywhere,Category="Setttings", meta=(AllowPrivateAccess=true))
	TSubclassOf<AEnemy> EnemyClass;

	float CounterToSpawnEnemy = 0;

protected:
	void PostEditTick(UWorld* World, float DeltaSeconds);
	virtual void BeginPlay() override;
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
	virtual void Tick(float DeltaTime) override;

private :
	void SpawnEnemy();
};
