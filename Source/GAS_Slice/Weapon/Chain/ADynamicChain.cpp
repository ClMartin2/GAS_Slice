#include "ADynamicChain.h"

UStaticMeshComponent* ADynamicChain::AddDynamicMesh(bool SimulatePhysics, bool _AngularBreakable, bool _LinearBreakable)
{
	UStaticMeshComponent* PreviousLastMesh = StaticMeshComponents[StaticMeshComponents.Num() - 1];

	if (AttachEnd && PreviousLastMesh != nullptr)
	{
		PreviousLastMesh->AttachToComponent(CustomRootComponent, FAttachmentTransformRules::KeepRelativeTransform);
		FVector NewLocation = LastPosition;
		PreviousLastMesh->SetRelativeLocation(NewLocation);
		PreviousLastMesh->SetSimulatePhysics(SimulatePhysics);
	}

	UStaticMeshComponent* StaticMeshComponent = CreateStaticMesh(!AttachEnd, false);
	
	if (CurrentIndex % 2 != 0)
		StaticMeshComponent->SetRelativeRotation(FRotator(0, 0, -90));

	if (CurrentIndex < 2)
		return nullptr;

	FVector NewLocation = LastPosition - FVector(
									(LengthStaticMesh + OffsetStaticMesh) - (LengthStaticMesh / 2 + OffsetPhysicConstraint.X)
									, OffsetPhysicConstraint.Y, OffsetPhysicConstraint.Z);

	UPhysicsConstraintComponent* PhysicsConstraintComponent = CreatePhysicsConstraint(
		StaticMeshComponents[CurrentIndex - 2],
		StaticMeshComponents[CurrentIndex - 1], NewLocation, true);

	PhysicsConstraintComponent->SetAngularBreakable(false, AngularBreakThreshold);
	PhysicsConstraintComponent->SetLinearBreakable(false, LinearBreakableThreshold);
	
	SetAngularBreakable(_AngularBreakable,_LinearBreakable);
	
	DynamicStaticMeshComponents.Add(StaticMeshComponent);
	DynamicPhysicsConstraintComponent.Add(PhysicsConstraintComponent);
	DynamicMeshToPhysicsConstraint.Add(StaticMeshComponent, PhysicsConstraintComponent);

	return StaticMeshComponent;
}

void ADynamicChain::DestroyDynamicMesh(UStaticMeshComponent* StaticMeshComponent, bool ClearDynamicArray)
{
	UPhysicsConstraintComponent* PhysicsConstraintComponent = DynamicMeshToPhysicsConstraint[StaticMeshComponent];
	
	StaticMeshComponents.Remove(StaticMeshComponent);
	PhysicsConstraints.Remove(PhysicsConstraintComponent);

	if (ClearDynamicArray)
	{
		DynamicStaticMeshComponents.Remove(StaticMeshComponent);
		DynamicPhysicsConstraintComponent.Remove(PhysicsConstraintComponent);
	}
	
	StaticMeshComponent->DestroyComponent();
	PhysicsConstraintComponent->DestroyComponent();
	CurrentIndex = StaticMeshComponents.Num();
	LastPosition = (CurrentIndex - 1) * FVector(LengthStaticMesh + OffsetStaticMesh, 0, 0);
	
	//Correction when set new end mesh to attach this to the root and correct is rotation
	UStaticMeshComponent* NewLastMesh = StaticMeshComponents[StaticMeshComponents.Num() - 1];
	
	if (AttachEnd)
	{
		AttachEndStaticMesh();
		NewLastMesh->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::KeepWorldTransform);
		if (CurrentIndex % 2 != 0)
			NewLastMesh->SetRelativeRotation(FRotator(0, 0, -90));
	}
}

void ADynamicChain::DestroyAllDynamicMeshes()
{
	for (int i = 0; i < DynamicStaticMeshComponents.Num(); i++)
	{
		DestroyDynamicMesh(DynamicStaticMeshComponents[i], false);
	}

	DynamicStaticMeshComponents.Empty();
	DynamicPhysicsConstraintComponent.Empty();
}

