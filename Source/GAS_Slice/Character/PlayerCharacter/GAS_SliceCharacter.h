// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "GAS_Slice/Character/BaseCharacter.h"
#include "GAS_SliceCharacter.generated.h"

class UInputComponent;
class USkeletalMeshComponent;
class UCameraComponent;
class UInputAction;
class UInputMappingContext;
class AKnife;
class UCableComponent;
class UChildActorComponent;
class UAC_LedgeGrab;

struct FInputActionValue;

DECLARE_DELEGATE(FDelegateCallBackChangeMappingContext);

UCLASS(config=Game)
class AGAS_SliceCharacter : public ABaseCharacter
{
	GENERATED_BODY()

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Mesh, meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* Mesh1P;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FirstPersonCameraComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Knife", meta = (AllowPrivateAccess = "true"))
	AKnife* Knife;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Knife", meta = (AllowPrivateAccess = "true"))
	USceneComponent* ParentKnife;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Spell", meta = (AllowPrivateAccess = "true"))
	USceneComponent* SpawnSpellPosition;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Mesh, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* HandStart;
	
	UPROPERTY(VisibleAnywhere,Category="Actor Component", meta=(AllowPrivateAccess = "true"))
	UAC_LedgeGrab* LedgeGrabComponent;

	UPROPERTY(VisibleAnywhere,Category="Actor Component", meta=(AllowPrivateAccess = "true"))
	UArrowComponent* DirectionAnimationKnife;
	
	FVector KnifeStartLocation;
	FRotator KnifeStartRotation;
	
	UChildActorComponent* KnifeChildActorComponent;
	
public:
	AGAS_SliceCharacter();
	
	USkeletalMeshComponent* GetMesh1P() const { return Mesh1P; }
	UCameraComponent* GetFirstPersonCameraComponent() const { return FirstPersonCameraComponent; }
	AKnife* GetKnife() const { return Knife; }
	UStaticMeshComponent* GetHandStart() const {return HandStart;}
	UAC_LedgeGrab* GetLedgeGrab() const {return LedgeGrabComponent;}
	UArrowComponent* GetDirectionAnimationKnife() const {return DirectionAnimationKnife;}
	UAbilitySystemComponent* _GetAbilitySystemComponent() const {return GetAbilitySystemComponent();}
};

