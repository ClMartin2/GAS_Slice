// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Knife.generated.h"
#include "Components/BoxComponent.h"

/**
 * 
 */
UCLASS()
class GAS_SLICE_API AKnife : public AActor
{
	GENERATED_BODY()

public:
	AKnife();	
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Knife")
	void StartMove(FVector DirectionThrowKnife);

	virtual void StartMove_Implementation(FVector DirectionThrowKnife);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Knife")
	void StopMove();

	virtual void StopMove_Implementation();


private:
	UFUNCTION(BlueprintCallable, Category = "Knife")
	void Move(float Acceleration);

private:
	FVector Direction;

	UPROPERTY(EditAnywhere, Category = "Settings|Speed")
	float SpeedKnife;

	UBoxComponent* BoxCollision = nullptr;
};
