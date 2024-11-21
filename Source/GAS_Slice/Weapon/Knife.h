// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Knife.generated.h"

class UProjectileMovementComponent;
class UBoxComponent;

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

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Knife")
	void Reset();

	virtual void Reset_Implementation();


private:
	UFUNCTION(BlueprintCallable, Category = "Knife")
	void Move(float Acceleration);

	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

private:
	FVector Direction;

	UPROPERTY(EditAnywhere, Category = "Settings|Speed")
	float Speed;

	UPROPERTY(EditAnywhere, Category = "Settings|Speed")
	float MaxSpeed;

	UPROPERTY(EditAnywhere, Category = Collision, meta = (AllowPrivateAccess = "true"))
	UBoxComponent* BoxCollision = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	UProjectileMovementComponent* ProjectileMovement;
};
