// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AC_LedgeGrab.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GAS_SLICE_API UAC_LedgeGrab : public UActorComponent
{
	GENERATED_BODY()

public:	
	UAC_LedgeGrab();

private:
	UPROPERTY(EditAnywhere,Category="Settings",meta=(AllowPrivateAccess=true))
	float LedgeGrabHeight = 10;

	UPROPERTY(EditAnywhere,Category="Settings",meta=(AllowPrivateAccess=true))
	float DistanceForward = 50;
	
	UPROPERTY(EditAnywhere,Category="Settings",meta=(AllowPrivateAccess=true))
	float CapsuleHalfHeight = 50.0f;

	UPROPERTY(EditAnywhere,Category="Settings",meta=(AllowPrivateAccess=true))
	float CapsuleRadius = 25.0f;

	UPROPERTY(EditAnywhere,Category="Settings",meta=(AllowPrivateAccess=true))
	FRotator CapsuleRotation = FRotator::ZeroRotator;

	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Settings",meta=(AllowPrivateAccess=true))
	UCurveFloat* CurveLerpGrabLedge;

	ACharacter* CharacterOwner;
	bool LerpingToLedge = false;
	
	
public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
protected:
	virtual void BeginPlay() override;
	virtual void OnRegister() override;

private:
	void CheckParentIsACharacter();
};
