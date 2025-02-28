// Fill out your copyright notice in the Description page of Project Settings.


#include "../Spells/WindPush.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/Character.h"
#include "DrawDebugHelpers.h"

void AWindPush::BeginPlay()
{
	Super::BeginPlay();
	
	ACharacter* OwnerCharacter = Cast<ACharacter>(GetOwner());
	APlayerController* PC = Cast<APlayerController>(OwnerCharacter->GetController());
	FVector ForwardCam = PC->PlayerCameraManager->GetCameraRotation().Vector();
	FVector End = Owner->GetActorLocation() + ForwardCam * 250;
	
	FVector NewVelocity = (Owner->GetActorLocation() - End).GetSafeNormal() * PushForce;

	DrawDebugDirectionalArrow(GetWorld(),Owner->GetActorLocation(), End,
		10,FColor::Red,true);
	
	OwnerCharacter->LaunchCharacter(NewVelocity,false,true);
}
