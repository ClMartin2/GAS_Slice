// Fill out your copyright notice in the Description page of Project Settings.


#include "Knife.h"

AKnife::AKnife()
{
	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));
	SetRootComponent(BoxCollision);
}

void AKnife::StartMove_Implementation(FVector DirectionThrowKnife)
{
	Direction = DirectionThrowKnife;
}

void AKnife::StopMove_Implementation()
{

}

void AKnife::Move(float Acceleration) {
	FVector NewLocation = GetActorLocation() + Direction * (SpeedKnife * Acceleration * GetWorld()->GetDeltaSeconds());
	SetActorLocation(NewLocation, true);
}
