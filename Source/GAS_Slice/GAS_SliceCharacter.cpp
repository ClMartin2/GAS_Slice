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
#include <Kismet/KismetStringLibrary.h>


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
		TArray<AActor*> ChildActors;
		GetAllChildActors(ChildActors, true);
		
		for (AActor* Actor : ChildActors)
		{			
			if (Actor->IsA(AKnife::StaticClass()))
			{
				Knife = Cast<AKnife>(Actor);
			}
		}
		
		if (Knife != nullptr) {
			UChildActorComponent* KnifeChildActor = Knife->GetParentComponent();
			ParentKnife = KnifeChildActor->GetAttachParent();
			KnifeStartLocation = KnifeChildActor->GetRelativeLocation();
			KnifeStartRotation = KnifeChildActor->GetRelativeRotation();
		}
	}
}

void AGAS_SliceCharacter::ResetKnife_Implementation()
{
	Knife->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	bool SuccesAttachement = Knife->AttachToComponent(ParentKnife, FAttachmentTransformRules::KeepWorldTransform);
	Knife->SetActorRelativeTransform(FTransform(KnifeStartRotation, KnifeStartLocation));
}

void AGAS_SliceCharacter::ThrowKnife_Implementation()
{
	Knife->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
}
