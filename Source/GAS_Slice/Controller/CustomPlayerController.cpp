// Fill out your copyright notice in the Description page of Project Settings.


#include "CustomPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "../Weapon/Knife.h"
#include "../GAS_SliceCharacter.h"
#include "DrawDebugHelpers.h"


ACustomPlayerController::ACustomPlayerController()
{

}

void ACustomPlayerController::BeginPlay()
{
	Super::BeginPlay();
	PlayerCharacter = (AGAS_SliceCharacter*)UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);

	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(DefaultMappingContext, 0);
	}

	if (PlayerCharacter != nullptr) {
		SetupPlayerInputComponent(PlayerCharacter->InputComponent);
		Knife = PlayerCharacter->GetKnife();

		if (Knife != nullptr)
			KnifeChildActor = Knife->GetParentComponent();
	}
}

void ACustomPlayerController::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	// add movement 
	PlayerCharacter->AddMovementInput(PlayerCharacter->GetActorForwardVector(), MovementVector.Y);
	PlayerCharacter->AddMovementInput(PlayerCharacter->GetActorRightVector(), MovementVector.X);	
}

void ACustomPlayerController::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	// add yaw and pitch input to controller
	PlayerCharacter->AddControllerYawInput(LookAxisVector.X);
	PlayerCharacter->AddControllerPitchInput(LookAxisVector.Y);
}

void ACustomPlayerController::Jump() {
	if (PlayerCharacter != nullptr) {
		PlayerCharacter->Jump();
	}
}

void ACustomPlayerController::StopJumping() {
	if (PlayerCharacter != nullptr) {
		PlayerCharacter->StopJumping();
	}
}

void ACustomPlayerController::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACustomPlayerController::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACustomPlayerController::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ACustomPlayerController::Move);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ACustomPlayerController::Look);

		//Throwing knife
		EnhancedInputComponent->BindAction(ThrowKnifeAction, ETriggerEvent::Triggered, this, &ACustomPlayerController::ThrowKnife);

		//Reset knife
		EnhancedInputComponent->BindAction(ResetKnifeAction, ETriggerEvent::Triggered, this, &ACustomPlayerController::ResetKnife);
	}
}

void ACustomPlayerController::ThrowKnife_Implementation()
{
	if (WasTheKnifeThrown)
		return;

	FVector ForwardThrowKnife = PlayerCameraManager->GetCameraRotation().Vector();
	
	FHitResult HitResult(ForceInit);
	FVector CameraLocation = PlayerCameraManager->GetCameraLocation();
	FVector KnifeLocation = Knife->GetActorLocation();

	FCollisionQueryParams RV_TraceParams = FCollisionQueryParams(FName(TEXT("RV_Trace")), true, this);
	RV_TraceParams.bTraceComplex = true;
	RV_TraceParams.bReturnPhysicalMaterial = false;

	GetWorld()->LineTraceSingleByChannel(HitResult, CameraLocation, CameraLocation + ForwardThrowKnife * 99999999999999999,ECollisionChannel::ECC_Visibility, RV_TraceParams);
	
	FVector DirectionKnife = FVector::ZeroVector;

	if (HitResult.GetActor() != nullptr)
		DirectionKnife = HitResult.ImpactPoint - KnifeLocation;
	else
		DirectionKnife = (CameraLocation + ForwardThrowKnife * 10000) - KnifeLocation;

	DirectionKnife = DirectionKnife.GetSafeNormal();
	
	PlayerCharacter->ThrowKnife();
	Knife->Throw(DirectionKnife,ForwardThrowKnife);
	
	WasTheKnifeThrown = true;
} 

void ACustomPlayerController::ResetKnife_Implementation()
{
	if (!WasTheKnifeThrown)
		return;

	PlayerCharacter->ResetKnife();
	Knife->ResetKnife();
	WasTheKnifeThrown = false;
}


