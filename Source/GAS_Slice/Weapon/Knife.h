// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Knife.generated.h"

class UProjectileMovementComponent;
class UBoxComponent;
class UStaticMeshComponent;
class UCableComponent;

/**
 * 
 */
UCLASS()
class GAS_SLICE_API AKnife : public AActor
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, Category = "Settings|Speed")
	float Speed;

	UPROPERTY(EditAnywhere, Category = "Settings|Speed")
	float MaxSpeed;

	UPROPERTY(EditAnywhere, Category = "Settings|Speed")
	float RotationSpeed;

	UPROPERTY(VisibleAnywhere, Category = Collision, meta = (AllowPrivateAccess = "true"))
	UBoxComponent* BoxCollision;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly, Category = "Visual", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* StaticMeshKnife_;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	UProjectileMovementComponent* ProjectileMovement;

public:
	AKnife();	
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Knife")
	void StartMove(FVector DirectionThrowKnife);

	virtual void StartMove_Implementation(FVector DirectionThrowKnife);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Knife")
	void Reset();

	virtual void Reset_Implementation();

protected:

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Knife")
	void StopMove();

	virtual void StopMove_Implementation();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Knife")
	void HitKnife(AActor* HitActor, FHitResult Hit);

	virtual void HitKnife_Implementation(AActor* HitActor, FHitResult Hit);


private:
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	UFUNCTION(BlueprintCallable, Category = "Knife")
	void RotateThrow();
};
