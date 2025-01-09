// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "AI/Navigation/NavAgentInterface.h"
#include "CustomPlayerController.generated.h"

class UInputComponent;
class UInputAction;
class UInputMappingContext;
class AKnife;
class AGAS_SliceCharacter;

struct FInputActionValue;

/**
 * 
 */
UCLASS()
class GAS_SLICE_API ACustomPlayerController : public APlayerController
{
	GENERATED_BODY()

private: 
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Character, meta = (AllowPrivateAccess = "true"))
	AGAS_SliceCharacter* PlayerCharacter;
	
#pragma region Mapping

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* DebugModeMappingContext;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* JumpAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* GoUpAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* MoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* ThrowKnifeAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* ResetKnifeAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* LookAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* DebugModeAction;
	
#pragma endregion Mapping

#pragma region DebugSettings
	UPROPERTY(EditAnywhere,Category="Settings|Debug", meta = (AllowPrivateAccess = "true"));
	float SpeedUpDebugMode = 2000;

	UPROPERTY(EditAnywhere,Category="Settings|Debug", meta = (AllowPrivateAccess = "true"));
	float SpeedForwardDebugMode = 3000;
#pragma endregion DebugSettings

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Knife", meta = (AllowPrivateAccess = "true"))
	AKnife* Knife;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Knife", meta = (AllowPrivateAccess = "true"))
	USceneComponent* KnifeChildActor;

	UPROPERTY(EditAnywhere, Category = "Settings|Knife", meta = (AllowPrivateAccess = "true"))
	float MaxDistance;
	
	UPROPERTY(EditAnywhere, Category = "Settings|Force", meta = (AllowPrivateAccess = "true"))
	float MaxZPushForce;

	UPROPERTY(EditAnywhere, Category = "Settings|Force", meta = (AllowPrivateAccess = "true"))
	float MinZPushForce;
	
	UPROPERTY(EditAnywhere, Category = "Settings|Force", meta = (AllowPrivateAccess = "true"))
	float MaxAngle = 90;

	UPROPERTY(EditAnywhere, Category = "Settings|Force", meta = (AllowPrivateAccess = "true"))
	float MaxPushForce = 2000;

	UPROPERTY(EditAnywhere, Category = "Settings|Force", meta = (AllowPrivateAccess = "true"))
	float MinPushForce = 100;

	UPROPERTY(EditAnywhere,BlueprintReadOnly, Category = "Settings|Force", meta = (AllowPrivateAccess = "true"))
	float ReducePushForceWhenLanded = 50;

	UPROPERTY(EditAnywhere,BlueprintReadOnly, Category = "Settings|Force", meta = (AllowPrivateAccess = "true"))
	float TimeToReduceForceWhenLanded = 0.25;

	float CounterTimeReduceForceWhenLanded;
	float ActualPushForce = MinPushForce;
	bool WasTheKnifeThrown = false;
	bool DebugModeActivated = false;

public:
	ACustomPlayerController();
	
protected:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Knife")
	void ThrowKnife();
	virtual void ThrowKnife_Implementation();
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Knife")
	void ResetKnife();
	virtual void ResetKnife_Implementation();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Knife")
	void PullKnife();
	virtual void PullKnife_Implementation();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Knife")
	void OnLandedCharacter();
	virtual void OnLandedCharacter_Implementation();

	UFUNCTION(BlueprintCallable, Category = "Character")
	void UpdateLandedCharacter();
	
	UFUNCTION()
	void LandedDelegate(const FHitResult& Hit);
	
	virtual void BeginPlay() override;
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void GoUp(const FInputActionValue& Value);
	void Jump();
	void StopJumping();
	void SetupDebugModeInputComponent();
	void SetUpPlayerInputComponent();
	void ActivateDebugMode();
	void ChangeMappingContext(UInputMappingContext* RemoveMappingContext, UInputMappingContext* AddMappingContext, TDelegate<void()>
	                          DelegateChangeMappingContexte, EMovementMode
	                          MovementMode);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Knife")
	void CheckDistanceKnife();
	virtual void CheckDistanceKnife_Implementation();

private:
	UFUNCTION()
	void PushToKnife();

	void SetActualPushForce(float ForcetoAdd)
	{
		ActualPushForce += /*FMath::Abs(*/ForcetoAdd/*)*/;
		ActualPushForce = FMath::Clamp(ActualPushForce, MinPushForce, MaxPushForce);
	}
};

