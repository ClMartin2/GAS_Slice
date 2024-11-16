// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "AI/Navigation/NavAgentInterface.h"
#include "CustomPlayerController.generated.h"

class UInputComponent;
class UInputAction;
class UInputMappingContext;
class UKnife;
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

	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;
	
	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* JumpAction;
	
	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* MoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* ThrowKnifeAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* ResetKnifeAction;
	
	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* LookAction;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component", meta = (AllowPrivateAccess = "true"))
	UKnife* Knife;

	UPROPERTY(EditAnywhere, Category = "Settings|Physics", meta = (AllowPrivateAccess = "true"))
	float ForceThrowKnife;

private:
	void ThrowKnife();
	void ResetKnife();

protected: 
	virtual void BeginPlay() override;

	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);

	void Jump();

	void StopJumping();

	void SetupPlayerInputComponent(UInputComponent* InputComponent);
	
public:
	ACustomPlayerController();
};
