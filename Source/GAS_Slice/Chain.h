// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Chain.generated.h"

class UPhysicsConstraintComponent;

UCLASS()
class GAS_SLICE_API AChain : public AActor
{
	GENERATED_BODY()
	
public:	
	AChain();

private:
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Settings",meta=(AllowPrivateAccess=true))
	int NumberOfLinks = 10;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Settings",meta=(AllowPrivateAccess=true))
	TArray<UStaticMeshComponent*> StaticMeshComponents;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Settings",meta=(AllowPrivateAccess=true))
	TArray<UPhysicsConstraintComponent*> PhysicsConstraints;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Settings",meta=(AllowPrivateAccess=true))
	USceneComponent* CustomRootComponent;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Settings",meta=(AllowPrivateAccess=true))
	float OffsetStaticMesh;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Settings",meta=(AllowPrivateAccess=true))
	float OffsetPhysicConstraint;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Settings",meta=(AllowPrivateAccess=true))
	FVector Scale = FVector::One();
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Settings",meta=(AllowPrivateAccess=true))
	UStaticMesh* StaticMesh;
protected:
	virtual void OnConstruction(const FTransform& Transform) override;
	void CustomDestroyConstructedComponents();;
};
