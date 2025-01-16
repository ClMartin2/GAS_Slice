#include "../Actor_Component/AC_LedgeGrab.h"
#include "KismetTraceUtils.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/Actor.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

UAC_LedgeGrab::UAC_LedgeGrab()
{
	PrimaryComponentTick.bCanEverTick = true;
	CheckParentIsACharacter();
}

void UAC_LedgeGrab::BeginPlay()
{
	Super::BeginPlay();
	CharacterOwner = Cast<ACharacter>(GetOwner());
}

void UAC_LedgeGrab::OnRegister()
{
	Super::OnRegister();
	CheckParentIsACharacter();
}

void UAC_LedgeGrab::CheckParentIsACharacter()
{
	if (GetOwner() && !GetOwner()->IsA<ACharacter>())
	{
		UE_LOG(LogTemp, Warning, TEXT("Le composant a été supprimé car l'acteur parent n'est pas un ACharacter."));
		DestroyComponent();
	}
}

void UAC_LedgeGrab::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!LerpLedgeGrab)
		UpdateCheckCollision();
	else
		UpdateTimerLedgeGrab();
}

void UAC_LedgeGrab::UpdateCheckCollision()
{
	FCollisionQueryParams Params(FName(TEXT("CapsuleTraceSingle")), true,GetOwner());
	Params.bReturnPhysicalMaterial = true;
	Params.bDebugQuery = true;

	FVector Start = CharacterOwner->GetPawnViewLocation();
	FVector End = Start - FVector::UpVector * LedgeGrabHeight;
	FVector VectorMoveForward = GetOwner()->GetActorForwardVector() * DistanceForward;
	
	Start += VectorMoveForward;
	End += VectorMoveForward;
	
	FHitResult OutHit;
	
	GetWorld()->SweepSingleByChannel(OutHit, Start, End, CapsuleRotation.Quaternion(),ECC_Visibility
	, FCollisionShape::MakeCapsule(CapsuleRadius,CapsuleHalfHeight), Params);

	if (DrawCapsule)
	{
		//Debug Draw Capsule
		DrawDebugCapsuleTraceSingle(GetWorld(),Start,End,CapsuleRadius,CapsuleHalfHeight,
		CapsuleRotation,EDrawDebugTrace::ForDuration,false,OutHit,FColor::Red,
		FColor::Green,0.1f);
	}

	bool MoveForward = FVector::DotProduct(GetOwner()->GetActorForwardVector(),
		CharacterOwner->GetCharacterMovement()->GetLastInputVector()) > 0;

	if (OutHit.bBlockingHit
		&& CharacterOwner->GetCharacterMovement()->IsWalkable(OutHit)
		&& CharacterOwner->GetCharacterMovement()->IsFalling() && MoveForward)
	{
		StartPositionLerpLedgeGrab = GetOwner()->GetActorLocation();
		EndPositionLerpLedgeGrab = OutHit.ImpactPoint + FVector::ZAxisVector *
			CharacterOwner->GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
		
		StartTimerLedgeGrab();
	}
}

void UAC_LedgeGrab::StartTimerLedgeGrab()
{
	CounterTimerLedgeGrab = 0;
	LerpLedgeGrab = true;
}

void UAC_LedgeGrab::UpdateTimerLedgeGrab()
{
	CounterTimerLedgeGrab += GetWorld()->GetDeltaSeconds();
	float Ratio = CounterTimerLedgeGrab / DurationToLerpToLedge;
	Ratio = CurveLerpGrabLedge->GetFloatValue(Ratio);
	
	FVector NewLocation = FMath::Lerp(StartPositionLerpLedgeGrab, EndPositionLerpLedgeGrab, Ratio);
	GetOwner()->SetActorLocation(NewLocation,false);
	GEngine->AddOnScreenDebugMessage(-1,1000,FColor::Red,
		FString::SanitizeFloat(CounterTimerLedgeGrab));

	if (Ratio >= 1)
	{
		LerpLedgeGrab = false;
	}
}

