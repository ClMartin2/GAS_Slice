// Fill out your copyright notice in the Description page of Project Settings.


#include "CustomPlayerController.h"

#include <string>

#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "../Weapon/Knife.h"
#include "../GAS_SliceCharacter.h"
#include "DrawDebugHelpers.h"
#include "MathUtil.h"
#include "../Library/ConvertLibrary.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetStringLibrary.h"

DECLARE_DELEGATE(FDelegateCallBackChangeMappingContext);

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
		SetUpPlayerInputComponent();
		Knife = PlayerCharacter->GetKnife();
		PlayerCharacter->LandedDelegate.AddDynamic(this, &ACustomPlayerController::LandedDelegate);
		
		if (Knife != nullptr)
			KnifeChildActor = Knife->GetParentComponent();

		InputComponent->BindKey(EKeys::G,IE_Pressed,this,&ACustomPlayerController::ActivateDebugMode);
	}
}

void ACustomPlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	GEngine->AddOnScreenDebugMessage(-1,0,FColor::Red,"Actual Push Force: "
		+ FString::SanitizeFloat(CurrentPushForce));

	GEngine->AddOnScreenDebugMessage(-1,0,FColor::Red,"Actual Speed: "
		+ FString::SanitizeFloat(CurrentSpeed));
}

#pragma region InputFunction

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

void ACustomPlayerController::GoUp(const FInputActionValue& Value)
{
	float LocalDirection = Value.Get<float>();
 	PlayerCharacter->AddMovementInput(FVector::UpVector,LocalDirection,false);
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

#pragma endregion InputFunction 

#pragma region SetUpInputFunction

void ACustomPlayerController::SetUpPlayerInputComponent()
{
	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent);
	
	EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACustomPlayerController::Jump);
	EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACustomPlayerController::StopJumping);
	EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ACustomPlayerController::Move);
	EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ACustomPlayerController::Look);
	EnhancedInputComponent->BindAction(ThrowKnifeAction, ETriggerEvent::Triggered, this, &ACustomPlayerController::ThrowKnife);
	EnhancedInputComponent->BindAction(ResetKnifeAction, ETriggerEvent::Triggered, this, &ACustomPlayerController::PullKnife);
}

void ACustomPlayerController::SetupDebugModeInputComponent()
{
	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent);
	
	EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ACustomPlayerController::Move);
	EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ACustomPlayerController::Look);
	EnhancedInputComponent->BindAction(GoUpAction, ETriggerEvent::Triggered, this, &ACustomPlayerController::GoUp);
}

void ACustomPlayerController::ChangeMappingContext(UInputMappingContext* RemoveMappingContext, UInputMappingContext*
	AddMappingContext,FDelegateCallBackChangeMappingContext DelegateChangeMappingContexte,EMovementMode MovementMode)
{
	UEnhancedInputLocalPlayerSubsystem* LocalSubSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	
	LocalSubSystem->RemoveMappingContext(RemoveMappingContext);
	LocalSubSystem->AddMappingContext(AddMappingContext, 0);
	DelegateChangeMappingContexte.Execute();
	PlayerCharacter->GetCharacterMovement()->SetMovementMode(MovementMode);
}

#pragma endregion SetUpInputFunction

void ACustomPlayerController::ActivateDebugMode()
{
	UEnhancedInputLocalPlayerSubsystem* LocalSubSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());

	if (!DebugModeActivated)
	{
		FDelegateCallBackChangeMappingContext DelegateSetDebugModeInput;
		DelegateSetDebugModeInput.BindUObject(this,&ACustomPlayerController::SetupDebugModeInputComponent);
		
		ChangeMappingContext(DefaultMappingContext,DebugModeMappingContext,
			DelegateSetDebugModeInput,MOVE_Flying);
	}
	else
	{
		FDelegateCallBackChangeMappingContext DelegateSetUpPlayerInput;
		DelegateSetUpPlayerInput.BindUObject(this,&ACustomPlayerController::SetUpPlayerInputComponent);
		
		ChangeMappingContext(DebugModeMappingContext,DefaultMappingContext,
			DelegateSetUpPlayerInput,MOVE_Falling);
	}

	DebugModeActivated = !DebugModeActivated;
}

#pragma region Knife

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

	GetWorld()->LineTraceSingleByChannel(HitResult, CameraLocation, CameraLocation + ForwardThrowKnife * 99999999999999999,
		ECollisionChannel::ECC_Visibility, RV_TraceParams);
	
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

void ACustomPlayerController::CheckDistanceKnife_Implementation()
{
	float Distance = FVector::Distance(PlayerCharacter->GetHandStart()->GetComponentLocation(),Knife->GetActorLocation());

	if (Distance > MaxDistance)
	{
		ResetKnife();
	}
}

void ACustomPlayerController::PushToKnife()
{
	if (Knife->GetIsAttached())
	{
		FVector LocalDirection = (Knife->GetActorLocation() - PlayerCameraManager->GetCameraLocation());
		float LengthVectorDirection = LocalDirection.Length();
		LocalDirection = LocalDirection.GetSafeNormal();
		
		float Angle = FMath::RadiansToDegrees(FMath::Acos(
			FVector::DotProduct(LocalDirection, PlayerCharacter->GetActorForwardVector())));
		float CoeffAngle = FVector::DotProduct(LocalDirection, -PlayerCharacter->GetActorUpVector());
		bool AddBaseZVelocity = ConvertLibrary::ConvertFloatToBoolNegativePositiveRange(-CoeffAngle);
		float LocalCoeffZpushForce = 1 - Angle/MaxAngle;

		SetActualPushForce(LengthVectorDirection * FMath::Abs(CoeffForceToAdd));

		float CoeffActualForce = CurrentPushForce/MaxPushForce;
		
		if (CurrentSpeed/MaxSpeed < CoeffActualForce)
			CurrentSpeed = FMath::Clamp(CoeffActualForce * MaxSpeed, MinSpeed, MaxSpeed);
		
		FVector LocalNewVelocity = LocalDirection * CurrentPushForce;
		
		LocalNewVelocity.Z = FMathf::Clamp(MinZPushForce * AddBaseZVelocity + LocalNewVelocity.Z
			,MinZPushForce,MaxZPushForce);

		PlayerCharacter->GetCharacterMovement()->Velocity = FVector::ZeroVector;
		PlayerCharacter->GetCharacterMovement()->AddImpulse(LocalNewVelocity, true);
	}
}

void ACustomPlayerController::SetActualPushForce(float ForceToAdd)
{
	CurrentPushForce += ForceToAdd;
	CurrentPushForce = FMath::Clamp(CurrentPushForce, MinPushForce, MaxPushForce);
}

void ACustomPlayerController::PullKnife_Implementation()
{
	PushToKnife();
	ResetKnife();
}

#pragma endregion Knife

#pragma region Landed

void ACustomPlayerController::LandedDelegate(const FHitResult& Hit)
{
	OnLandedCharacter();
}

void ACustomPlayerController::OnLandedCharacter_Implementation()
{
	
}
#pragma endregion Landed

void ACustomPlayerController::SetActualSpeed(float SpeedToAdd)
{
	CurrentSpeed += SpeedToAdd;
	CurrentSpeed = FMath::Clamp(CurrentSpeed, MinSpeed, MaxSpeed);
}

