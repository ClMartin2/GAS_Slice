#include "GAS_SliceCharacter.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Engine/LocalPlayer.h"
#include "../../Weapon/Knife.h"
#include "CableComponent.h"
#include "Components/ChildActorComponent.h"
#include "../../Component/ActorComponent/AC_LedgeGrab.h"
#include "Components/ArrowComponent.h"
#include "AbilitySystemComponent.h"
#include "../../GAS/AttributeSet/BasicAttributeSet.h"

AGAS_SliceCharacter::AGAS_SliceCharacter()
{
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);

	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->SetRelativeLocation(FVector(-10.f, 0.f, 60.f)); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	Mesh1P->SetOnlyOwnerSee(true);
	Mesh1P->SetupAttachment(FirstPersonCameraComponent);
	Mesh1P->bCastDynamicShadow = false;
	Mesh1P->CastShadow = false;
	Mesh1P->SetRelativeLocation(FVector(-30.f, 0.f, -150.f));

	HandStart = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("HandStart"));
	HandStart->SetupAttachment(FirstPersonCameraComponent);	

	DirectionAnimationKnife = CreateDefaultSubobject<UArrowComponent>(TEXT("DirectionAnimationKnife"));
	DirectionAnimationKnife->SetupAttachment(FirstPersonCameraComponent);

	SpawnSpellPosition = CreateDefaultSubobject<USceneComponent>(TEXT("Spawn Spell Position"));
	SpawnSpellPosition->SetupAttachment(FirstPersonCameraComponent);
	
	LedgeGrabComponent = CreateDefaultSubobject<UAC_LedgeGrab>(TEXT("Ledge Grab"));
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

		if (Knife != nullptr)
		{
			KnifeChildActorComponent = Knife->GetParentComponent();
			ParentKnife = KnifeChildActorComponent->GetAttachParent();
			KnifeStartLocation = KnifeChildActorComponent->GetRelativeLocation();
			KnifeStartRotation = KnifeChildActorComponent->GetRelativeRotation();
			Knife->SetAbilitySystemComponent(GetAbilitySystemComponent());
			Knife->SetHandStartLocation(HandStart);
		}
	}
}

void AGAS_SliceCharacter::BeginPlay()
{
	Super::BeginPlay();
	ResetKnife();
	Knife->GetBPChain()->SetComponentToAttachEnd(HandStart);
}

void AGAS_SliceCharacter::ResetKnife_Implementation()
{
	Knife->AttachToComponent(ParentKnife, FAttachmentTransformRules::KeepRelativeTransform);
	Knife->SetActorRelativeTransform(FTransform(KnifeStartRotation, KnifeStartLocation));
}

void AGAS_SliceCharacter::ThrowKnife_Implementation()
{
	Knife->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
}
