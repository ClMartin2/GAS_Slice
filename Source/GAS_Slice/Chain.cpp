// Fill out your copyright notice in the Description page of Project Settings.


#include "Chain.h"
#include "PhysicsEngine/PhysicsConstraintComponent.h"

AChain::AChain()
{
	PrimaryActorTick.bCanEverTick = true;
	CustomRootComponent = CreateDefaultSubobject<USceneComponent>(FName("RootComponent"));
	RootComponent = CustomRootComponent;
}

void AChain::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	DestroyConstructedComponents();
	CustomDestroyConstructedComponents();
	
	float LengthStaticMesh = (StaticMesh != nullptr) ? (StaticMesh->GetBounds().GetBox().GetSize() * Scale).X : 0.0f;

	TArray<UStaticMeshComponent*> ChainLinks;
	FVector LastPosition = FVector::ZeroVector; 

	for (int i = 0; i < NumberOfLinks; i++)
	{
		UStaticMeshComponent* StaticMeshComponent = NewObject<UStaticMeshComponent>(this);
		StaticMeshComponent->AttachToComponent(CustomRootComponent, FAttachmentTransformRules::KeepRelativeTransform);
		StaticMeshComponent->SetWorldScale3D(Scale);

		if (StaticMesh != nullptr)
			StaticMeshComponent->SetStaticMesh(StaticMesh);

		FVector NewPosition = LastPosition + FVector(LengthStaticMesh + OffsetStaticMesh, 0, 0);
		StaticMeshComponent->SetRelativeLocation(NewPosition);
		LastPosition = NewPosition; 

		if (i == 0 || (i + 1) % 2 != 0)
			StaticMeshComponent->SetRelativeRotation(FRotator(0, 0, -90));

		if (i != 0)
			StaticMeshComponent->SetSimulatePhysics(true);

		ChainLinks.Add(StaticMeshComponent);
		StaticMeshComponents.Add(StaticMeshComponent);
		
		StaticMeshComponent->RegisterComponent();
		
		if (i > 0)
		{
			UPhysicsConstraintComponent* PhysicsConstraint = NewObject<UPhysicsConstraintComponent>(this);
			PhysicsConstraint->RegisterComponent();
			PhysicsConstraint->AttachToComponent(CustomRootComponent, FAttachmentTransformRules::KeepRelativeTransform);
			PhysicsConstraint->SetConstrainedComponents(ChainLinks[i - 1], NAME_None, ChainLinks[i], NAME_None);
			PhysicsConstraints.Add(PhysicsConstraint);
			PhysicsConstraint->SetRelativeLocation(LastPosition - FVector((LengthStaticMesh + OffsetStaticMesh) -
				(LengthStaticMesh/2 + OffsetPhysicConstraint) , 0, 0));
			PhysicsConstraint->SetDisableCollision(true);
		}
	}
}

void AChain::CustomDestroyConstructedComponents()
{
	// Destroy existing StaticMeshComponents
	for (UStaticMeshComponent* Component : StaticMeshComponents)
	{
		if (Component)
		{
			Component->DestroyComponent();
		}
	}
	StaticMeshComponents.Empty();

	// Destroy any previously created physics constraints
	for (UPhysicsConstraintComponent* Constraint : PhysicsConstraints)
	{
		if (Constraint)
		{
			Constraint->DestroyComponent();
		}
	}
	PhysicsConstraints.Empty();
}


