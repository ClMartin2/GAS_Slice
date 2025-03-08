#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PhysicsCore.h"
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
	bool Reset = false;

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

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Settings",meta=(AllowPrivateAccess=true))
	float LinearDampling = 0.01f;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Settings",meta=(AllowPrivateAccess=true))
	float AngularDampling = 0;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Settings",meta=(AllowPrivateAccess=true))
	bool AttachStart = true;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Settings",meta=(AllowPrivateAccess=true))
	bool AttachEnd = false;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Settings",meta=(AllowPrivateAccess=true))
	FString AttachEndComponentName;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Settings|PhysicsConstraint",meta=(AllowPrivateAccess=true))
	TEnumAsByte<EAngularConstraintMotion> ConstraintTwistMotion;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Settings|PhysicsConstraint",meta=(AllowPrivateAccess=true))
	float TwistLimitAngle;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Settings|PhysicsConstraint",meta=(AllowPrivateAccess=true))
	TEnumAsByte<EAngularConstraintMotion> ConstraintSwing1Motion;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Settings|PhysicsConstraint",meta=(AllowPrivateAccess=true))
	float Swing1LimitAngle;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Settings|PhysicsConstraint",meta=(AllowPrivateAccess=true))
	TEnumAsByte<EAngularConstraintMotion> ConstraintSwing2Motion;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Settings|PhysicsConstraint",meta=(AllowPrivateAccess=true))
	float Swing2LimitAngle;
	
protected:
	virtual void OnConstruction(const FTransform& Transform) override;
	void CustomDestroyConstructedComponents();

private:
	void CreatePhysicsConstraint(UPrimitiveComponent* FirstComponent, UPrimitiveComponent* SecondComponent, FVector Location, bool
	                             IsRelativeLocation = true);
};
