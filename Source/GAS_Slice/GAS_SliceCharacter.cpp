// Copyright Epic Games, Inc. All Rights Reserved.

#include "GAS_SliceCharacter.h"
#include "GAS_SliceProjectile.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "Engine/LocalPlayer.h"
#include "Weapon/Knife.h"


AGAS_SliceCharacter::AGAS_SliceCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);
		
	// Create a CameraComponent	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->SetRelativeLocation(FVector(-10.f, 0.f, 60.f)); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	Mesh1P->SetOnlyOwnerSee(true);
	Mesh1P->SetupAttachment(FirstPersonCameraComponent);
	Mesh1P->bCastDynamicShadow = false;
	Mesh1P->CastShadow = false;
	Mesh1P->SetRelativeLocation(FVector(-30.f, 0.f, -150.f));
}

void AGAS_SliceCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void AGAS_SliceCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	if (!Knife)
	{
		Knife = FindComponentByClass<UKnife>();
		ParentKnife = Knife->GetAttachParent();
	}
}

void AGAS_SliceCharacter::ResetKnife_Implementation()
{
	Knife->SetupAttachment(ParentKnife, NameSocketKnife);
}

void AGAS_SliceCharacter::ThrowKnife_Implementation()
{
	Knife->DetachFromParent(true);
}
