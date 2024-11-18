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

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Knife", meta = (AllowPrivateAccess = "true"))
	UKnife* Knife;

	UPROPERTY(EditAnywhere, Category = "Settings|Knife", meta = (AllowPrivateAccess = "true"))
	float ForceThrowKnife;

	FVector ForwardThrowKnife;

protected:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Knife")
	void ThrowKnife();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Knife")
	void ResetKnife();

	UFUNCTION(BlueprintCallable, Category = "Knife")
	void MoveKnife();

	virtual void ResetKnife_Implementation();
	virtual void ThrowKnife_Implementation();
	virtual void BeginPlay() override;
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void Jump();
	void StopJumping();
	void SetupPlayerInputComponent(UInputComponent* InputComponent);
	
public:
	ACustomPlayerController();
};
