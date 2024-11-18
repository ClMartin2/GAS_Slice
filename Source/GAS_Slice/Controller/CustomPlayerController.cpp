// Fill out your copyright notice in the Description page of Project Settings.


#include "CustomPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "../Weapon/Knife.h"
#include "../GAS_SliceCharacter.h"
#include <Kismet/KismetMathLibrary.h>


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
	PlayerCharacter->ThrowKnife();

	FRotator CameraRotation = PlayerCameraManager->GetCameraRotation();
	ForwardThrowKnife = CameraRotation.Vector();

	FRotator RotationToForward = UKismetMathLibrary::FindLookAtRotation(FVector::ZeroVector, ForwardThrowKnife);
	Knife->SetWorldRotation(RotationToForward);

	FHitResult Hit(ForceInit);
	FVector start = Knife->GetComponentLocation();
	FVector End = start + ForwardThrowKnife * 700.f;
	FCollisionQueryParams CollisionParams;
	DrawDebugLine(GetWorld(), start, End, FColor::Green, true, -1.0f, 0, 5.f);
} 

void ACustomPlayerController::MoveKnife()
{
	FVector NewLocation = Knife->GetComponentLocation() + ForwardThrowKnife * (ForceThrowKnife * GetWorld()->GetDeltaSeconds());
	Knife->SetWorldLocation(NewLocation,true);
}

void ACustomPlayerController::ResetKnife_Implementation()
{
	PlayerCharacter->ResetKnife();
	Knife->SetRelativeLocation(FVector::ZeroVector);
}


