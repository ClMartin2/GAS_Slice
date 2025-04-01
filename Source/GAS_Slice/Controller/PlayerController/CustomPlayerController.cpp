#include "CustomPlayerController.h"
#include "CableComponent.h"
#include "CollisionDebugDrawingPublic.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "../../Weapon/Knife.h"
#include "../../Character/PlayerCharacter/GAS_SliceCharacter.h"
#include "DrawDebugHelpers.h"
#include "MathUtil.h"
#include "Camera/CameraComponent.h"
#include "Components/ArrowComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

#pragma region Unreal Functions

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

		Knife->OnChainBreak.BindUObject(this, &ACustomPlayerController::OnBreakChain);

		//Debug
		// InputComponent->BindKey(EKeys::G,IE_Pressed,this,&ACustomPlayerController::ActivateDebugMode);

		GetPlayerCharacterMovement()->AirControl = BaseAirControlValue;
		StartLocationKnife = Knife->GetActorLocation();
	}

	FOnTimelineFloat ProgressUpdate;
	ProgressUpdate.BindUFunction(this,FName("AttackAnimationUpdate"));

	FOnTimelineEvent FinishedEvent;
	FinishedEvent.BindUFunction(this,FName("AttackAnimationFinish"));

	TimelineAttackAnimation.AddInterpFloat(CurveTimelineAttackAnimation,ProgressUpdate);
	TimelineAttackAnimation.SetTimelineFinishedFunc(FinishedEvent);
	TimelineAttackAnimation.SetPlayRate(1/DurationAnimAttack);
}

void ACustomPlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	TimelineAttackAnimation.TickTimeline(DeltaTime);
}

#pragma endregion	Unreal Functions

#pragma region InputFunction

void ACustomPlayerController::Move(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();
	
	PlayerCharacter->AddMovementInput(PlayerCharacter->GetActorForwardVector(), MovementVector.Y);
	PlayerCharacter->AddMovementInput(PlayerCharacter->GetActorRightVector(), MovementVector.X);	
}

void ACustomPlayerController::Look(const FInputActionValue& Value)
{
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	PlayerCharacter->AddControllerYawInput(LookAxisVector.X);
	PlayerCharacter->AddControllerPitchInput(LookAxisVector.Y);
}

void ACustomPlayerController::GoUp(const FInputActionValue& Value)
{
	float LocalDirection = Value.Get<float>();
 	PlayerCharacter->AddMovementInput(FVector::UpVector,LocalDirection,false);
}

void ACustomPlayerController::Jump() {
	if (JumpCount >= PlayerCharacter->JumpMaxCount)
		return;
	
	FCollisionQueryParams RV_TraceParams =
		FCollisionQueryParams(FName(TEXT("RV_Trace")), true, PlayerCharacter);
	RV_TraceParams.bTraceComplex = true;
	RV_TraceParams.bReturnPhysicalMaterial = false;

	FHitResult RV_Hit(ForceInit);

	FVector Start = PlayerCharacter->GetActorLocation() -
		PlayerCharacter->GetActorUpVector() * PlayerCharacter->GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
	FVector End = Start - PlayerCharacter->GetActorUpVector() * DistanceBuffedJump;
	
	if (RV_Hit.bBlockingHit)
	{
		GetPlayerCharacterMovement()->SetMovementMode(MOVE_Walking);
	}

	if (PlayerCharacter != nullptr) {
		PlayerCharacter->Jump();
		JumpCount++;
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
	EnhancedInputComponent->BindAction(AttackEnemyAction, ETriggerEvent::Triggered, this, &ACustomPlayerController::AttackEnemy);
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
	GetPlayerCharacterMovement()->SetMovementMode(MovementMode);
}
#pragma endregion SetUpInputFunction

void ACustomPlayerController::ActivateDebugMode()
{
	UEnhancedInputLocalPlayerSubsystem* LocalSubSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());

	if (!bDebugModeActivated)
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

	bDebugModeActivated = !bDebugModeActivated;
}

#pragma region Knife

void ACustomPlayerController::ThrowKnife_Implementation()
{
	if (bWasTheKnifeThrown && bIsAttacking)
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
	
	bWasTheKnifeThrown = true;
} 

void ACustomPlayerController::ResetKnife_Implementation()
{
	Knife->ResetKnife();
	PlayerCharacter->ResetKnife();
	
	bWasTheKnifeThrown = false;
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
	if (!Knife->GetIsAttached())
		return;
	
	FVector LocalDirection = (Knife->GetActorLocation() - PlayerCameraManager->GetCameraLocation());
	float LengthVectorDirection = LocalDirection.Length();
	LocalDirection = LocalDirection.GetSafeNormal();
	
	float Angle = FMath::RadiansToDegrees(FMath::Acos(
		FVector::DotProduct(LocalDirection, PlayerCharacter->GetActorForwardVector())));
	float CoeffAngle = FVector::DotProduct(LocalDirection, -PlayerCharacter->GetActorUpVector());
	bool AddBaseZVelocity = -CoeffAngle >= 0.0f;
	// float LocalCoeffZpushForce = 1 - Angle/MaxAngle;
	SetActualPushForce(LengthVectorDirection * FMath::Abs(CoeffForceToAdd));
	float CoeffActualForce = CurrentPushForce/MaxPushForce;
	
	if (CurrentSpeed/MaxSpeed < CoeffActualForce)
	{
		CurrentSpeed = FMath::Clamp(CoeffActualForce * MaxSpeed, MinSpeed, MaxSpeed);
		GetPlayerCharacterMovement()->MaxWalkSpeed = CurrentSpeed;
	}
	
	FVector LocalNewVelocity = LocalDirection * CurrentPushForce;
	
	LocalNewVelocity.Z = FMathf::Clamp(MinZPushForce * AddBaseZVelocity + LocalNewVelocity.Z
		,MinZPushForce,MaxZPushForce);

	GetPlayerCharacterMovement()->Velocity = FVector::ZeroVector;
	GetPlayerCharacterMovement()->AddImpulse(LocalNewVelocity, true);
	GetPlayerCharacterMovement()->AirControl = AirControlPushToKnife; 
}

void ACustomPlayerController::SetActualPushForce(float ForceToAdd)
{
	CurrentPushForce += ForceToAdd;
	CurrentPushForce = FMath::Clamp(CurrentPushForce, MinPushForce, MaxPushForce);
}

void ACustomPlayerController::PullKnife_Implementation()
{
	if (!bWasTheKnifeThrown && bIsAttacking)
		return;

	PushToKnife();
	ResetKnife();
}

void ACustomPlayerController::OnBreakChain()
{
	ResetKnife();
}

#pragma endregion Knife

#pragma region Landed

void ACustomPlayerController::LandedDelegate(const FHitResult& Hit)
{
	GetPlayerCharacterMovement()->AirControl = BaseAirControl;
	JumpCount = 0;
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
	GetPlayerCharacterMovement()->MaxWalkSpeed = CurrentSpeed;
}

UCharacterMovementComponent* ACustomPlayerController::GetPlayerCharacterMovement()
{
	if (PlayerCharacter != nullptr)
		return PlayerCharacter->GetCharacterMovement();
	else
		return nullptr;
}

FGenericTeamId ACustomPlayerController::GetGenericTeamId() const
{
	return 	FGenericTeamId(TeamId);
}

#pragma region Attack

void ACustomPlayerController::AttackEnemy_Implementation()
{
	if (bIsAttacking && bWasTheKnifeThrown)
		return;
	
	FVector StartLocation = PlayerCharacter->GetDirectionAnimationKnife()->GetRelativeLocation();
	FRotator StartRotation = RotationAnimAttack;
	
	Knife->SetActorLocation(StartLocation,false,nullptr,ETeleportType::ResetPhysics);
	Knife->SetActorRelativeRotation(StartRotation,false,nullptr,ETeleportType::ResetPhysics);

	StartLocationKnifeAttackAnim = StartLocation;
	StartRotationKnifeAttackAnim = StartRotation;
	
	TimelineAttackAnimation.PlayFromStart();
	GetWorldTimerManager().SetTimer(UpdateAttackTimerHandle, this, &ACustomPlayerController::CheckCollisionAttack,
		0.01f, true);
	bIsAttacking = true;
}


void ACustomPlayerController::AttackAnimationUpdate(float Ratio) const
{
	FVector StartLocation = StartLocationKnifeAttackAnim;
	FVector LocalForwardArrowVector = UKismetMathLibrary::InverseTransformDirection(PlayerCharacter->GetFirstPersonCameraComponent()->GetComponentTransform()
		,PlayerCharacter->GetDirectionAnimationKnife()->GetForwardVector());
	FVector EndLocation = StartLocation + LocalForwardArrowVector * DistanceAttackAnim;

	FRotator StartRotation = StartRotationKnifeAttackAnim;
	FRotator EndRotation = StartRotation + EndRotationAnimAttack;

	FRotator LerpRotation = FMath::Lerp(StartRotation,EndRotation,Ratio);
	FVector LerpLocation = FMath::Lerp(StartLocation,EndLocation,Ratio);
	
	FTransform NewTransform = FTransform(LerpRotation,LerpLocation,Knife->GetActorScale());
	
	Knife->SetActorRelativeTransform(NewTransform,false,nullptr,ETeleportType::ResetPhysics);
}

void ACustomPlayerController::AttackAnimationFinish()
{
	ResetKnife();
	bIsAttacking = false;
	Knife->FinishCheckCollisionAttack();
	GetWorldTimerManager().ClearTimer(UpdateAttackTimerHandle);
}

void ACustomPlayerController::CheckCollisionAttack() const
{
	Knife->CheckCollisionAttack();
}

#pragma endregion Attack
