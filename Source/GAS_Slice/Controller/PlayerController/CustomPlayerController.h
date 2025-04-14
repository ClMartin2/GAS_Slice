// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "AI/Navigation/NavAgentInterface.h"
#include "GenericTeamAgentInterface.h"
#include "Components/TimelineComponent.h"
#include "CustomPlayerController.generated.h"

class UCharacterMovementComponent;
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
class GAS_SLICE_API ACustomPlayerController : public APlayerController, public IGenericTeamAgentInterface
{
	GENERATED_BODY()
public:
	ACustomPlayerController() = default;
	
private: 
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Character, meta = (AllowPrivateAccess = "true"))
	AGAS_SliceCharacter* PlayerCharacter;
	
#pragma region Mapping

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings|Input", meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings|Input", meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* DebugModeMappingContext;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Settings|Input", meta=(AllowPrivateAccess = "true"))
	UInputAction* JumpAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Settings|Input", meta=(AllowPrivateAccess = "true"))
	UInputAction* GoUpAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Settings|Input", meta=(AllowPrivateAccess = "true"))
	UInputAction* MoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings|Input", meta = (AllowPrivateAccess = "true"))
	UInputAction* ThrowKnifeAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings|Input", meta = (AllowPrivateAccess = "true"))
	UInputAction* ResetKnifeAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings|Input", meta = (AllowPrivateAccess = "true"))
	UInputAction* LookAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings|Input", meta = (AllowPrivateAccess = "true"))
	UInputAction* DebugModeAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings|Input", meta = (AllowPrivateAccess = "true"))
	UInputAction* AttackEnemyAction;
	
#pragma endregion Mapping

#pragma region DebugSettings
	UPROPERTY(EditAnywhere,Category="Settings|Debug", meta = (AllowPrivateAccess = "true"));
	float SpeedUpDebugMode = 2000;

	UPROPERTY(EditAnywhere,Category="Settings|Debug", meta = (AllowPrivateAccess = "true"));
	float SpeedForwardDebugMode = 3000;
#pragma endregion DebugSettings

	AKnife* Knife;
	USceneComponent* KnifeChildActor;
	USceneComponent* ParentKnife;
	FVector KnifeStartLocation;
	FRotator KnifeStartRotation;
	
	UPROPERTY(EditAnywhere, Category = "Settings|Knife", meta = (AllowPrivateAccess = "true"))
	float MaxDistance;

#pragma region ForceSettings

	UPROPERTY(EditAnywhere, Category = "Settings|Force", meta = (AllowPrivateAccess = "true"))
	float MaxZPushForce;

	UPROPERTY(EditAnywhere, Category = "Settings|Force", meta = (AllowPrivateAccess = "true"))
	float MinZPushForce;
	
	UPROPERTY(EditAnywhere, Category = "Settings|Force", meta = (AllowPrivateAccess = "true"))
	float MaxAngle = 90;

	UPROPERTY(EditAnywhere, Category = "Settings|Force", meta = (AllowPrivateAccess = "true"))
	float MaxPushForce = 2000;

	UPROPERTY(EditAnywhere,BlueprintReadOnly, Category = "Settings|Force", meta = (AllowPrivateAccess = "true"))
	float MinPushForce = 100;

	UPROPERTY(EditAnywhere,BlueprintReadOnly, Category = "Settings|Force", meta = (AllowPrivateAccess = "true"))
	float CoeffForceToAdd = 0.25;
#pragma endregion ForceSettings

	UPROPERTY(EditAnywhere,BlueprintReadOnly, Category = "Settings|Speed", meta = (AllowPrivateAccess = "true"))
	float MinCharacterSpeed = 800;

	UPROPERTY(EditAnywhere,BlueprintReadOnly, Category = "Settings|Speed", meta = (AllowPrivateAccess = "true"))
	float MaxCharacterSpeed = 2000;

	UPROPERTY(EditAnywhere,BlueprintReadOnly, Category = "Settings|Jump", meta = (AllowPrivateAccess = "true"))
    float DistanceBuffedJump = 50;

	UPROPERTY(EditAnywhere,BlueprintReadOnly, Category = "Settings|AirControl", meta = (AllowPrivateAccess = "true"))
	float BaseAirControl = 0.5;

	UPROPERTY(EditAnywhere,BlueprintReadOnly, Category = "Settings|AirControl", meta = (AllowPrivateAccess = "true"))
	float AirControlPushToKnife = 0.25;

	UPROPERTY(EditAnywhere,BlueprintReadOnly, Category = "Settings|Attack", meta = (AllowPrivateAccess = "true"))
	float DistanceAttackAnim = 30;

	UPROPERTY(EditAnywhere,BlueprintReadOnly, Category = "Settings|Attack", meta = (AllowPrivateAccess = "true"))
	float DurationAnimAttack = 0.2;

	UPROPERTY(EditAnywhere,BlueprintReadOnly, Category = "Settings|Attack", meta = (AllowPrivateAccess = "true"))
	FRotator RotationAnimAttack = FRotator::ZeroRotator;

	UPROPERTY(EditAnywhere,BlueprintReadOnly, Category = "Settings|Attack", meta = (AllowPrivateAccess = "true"))
	FRotator EndRotationAnimAttack = FRotator::ZeroRotator;

	UPROPERTY(EditAnywhere,BlueprintReadOnly, Category = "Settings|Attack", meta = (AllowPrivateAccess = "true"))
	UCurveFloat* CurveTimelineAttackAnimation = nullptr;

	UPROPERTY(EditAnywhere,BlueprintReadOnly, Category = "Settings|ID", meta = (AllowPrivateAccess = "true"))
	uint8 TeamId;

	UPROPERTY(EditAnywhere,BlueprintReadOnly, Category = "Settings|KnifeChain", meta = (AllowPrivateAccess = "true"))
	float DelayResetKnifeAfterBreaking = 0.1;
	
	UPROPERTY(BlueprintReadOnly, Category = "Speed", meta = (AllowPrivateAccess = "true"))
	float CurrentSpeed = MinCharacterSpeed;
	
	float CounterTimeReduceForceWhenLanded;
	float CurrentPushForce = MinPushForce;
	float BaseAirControlValue = 0;
	float JumpCount = 0;
	FVector StartLocationKnifeAttackAnim = FVector::Zero();
	FVector StartLocationKnife = FVector::Zero();
	FRotator StartRotationKnifeAttackAnim = FRotator::ZeroRotator;

	bool bWasTheKnifeThrown = false;
	bool bDebugModeActivated = false;
	bool bIsAttacking = false;
	bool bChainBreak = false;

	FTimeline TimelineAttackAnimation;
	FTimerHandle UpdateAttackTimerHandle;
	FTimerHandle ResetKnifeAfterBreakingChain;
	FTimerHandle StartDelayInitKnife;

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

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Knife")
	void AttackEnemy();
	virtual void AttackEnemy_Implementation();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	
	UFUNCTION(BlueprintCallable, Category = "Knife")
	void CheckDistanceKnife();

private:
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void GoUp(const FInputActionValue& Value);
	void Jump();
	void StopJumping();
	void SetupDebugModeInputComponent();
	void SetUpPlayerInputComponent();
	void ActivateDebugMode();
	void ChangeMappingContext(UInputMappingContext* RemoveMappingContext, UInputMappingContext* AddMappingContext, TDelegate<void()>
							  DelegateChangeMappingContexte, EMovementMode MovementMode);
	void OnBreakChain();

	UFUNCTION()
	void PushToKnife();

	UFUNCTION()
	void AttackAnimationUpdate(float Ratio) const;

	UFUNCTION()
	void AttackAnimationFinish();
	
	UFUNCTION()
	void LandedDelegate(const FHitResult& Hit);
	
	UFUNCTION(BlueprintCallable,Category="Force", meta = (AllowPrivateAccess = "true"))
	void SetActualPushForce(float ForceToAdd);
	
	UFUNCTION(BlueprintCallable,Category="Speed", meta = (AllowPrivateAccess = "true"))
	void SetActualSpeed(float SpeedToAdd);
	
	UFUNCTION(BlueprintCallable,Category="Player", meta = (AllowPrivateAccess = "true"))
	UCharacterMovementComponent* GetPlayerCharacterMovement();

	virtual FGenericTeamId GetGenericTeamId() const override;
	void CheckCollisionAttack() const;
	void InitKnife();
};

