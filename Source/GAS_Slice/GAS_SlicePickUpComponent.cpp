// Copyright Epic Games, Inc. All Rights Reserved.

#include "GAS_SlicePickUpComponent.h"

UGAS_SlicePickUpComponent::UGAS_SlicePickUpComponent()
{
	// Setup the Sphere Collision
	SphereRadius = 32.f;
}

void UGAS_SlicePickUpComponent::BeginPlay()
{
	Super::BeginPlay();

	// Register our Overlap Event
	OnComponentBeginOverlap.AddDynamic(this, &UGAS_SlicePickUpComponent::OnSphereBeginOverlap);
}

void UGAS_SlicePickUpComponent::OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// Checking if it is a First Person Character overlapping
	AGAS_SliceCharacter* Character = Cast<AGAS_SliceCharacter>(OtherActor);
	if(Character != nullptr)
	{
		// Notify that the actor is being picked up
		OnPickUp.Broadcast(Character);

		// Unregister from the Overlap Event so it is no longer triggered
		OnComponentBeginOverlap.RemoveAll(this);
	}
}
