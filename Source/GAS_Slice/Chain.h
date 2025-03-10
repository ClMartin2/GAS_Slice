#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PhysicsCore.h"
#include "PhysicsEngine/ConstraintDrives.h"
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

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Settings",meta=(AllowPrivateAccess=true))
	float Mass = 1;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Settings|PhysicsConstraint|AngularLimit",meta=(AllowPrivateAccess=true))
	TEnumAsByte<EAngularConstraintMotion> ConstraintTwistMotion;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Settings|PhysicsConstraint|AngularLimit",meta=(AllowPrivateAccess=true))
	float TwistLimitAngle;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Settings|PhysicsConstraint|AngularLimit",meta=(AllowPrivateAccess=true))
	TEnumAsByte<EAngularConstraintMotion> ConstraintSwing1Motion;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Settings|PhysicsConstraint|AngularLimit",meta=(AllowPrivateAccess=true))
	float Swing1LimitAngle;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Settings|PhysicsConstraint|AngularLimit",meta=(AllowPrivateAccess=true))
	TEnumAsByte<EAngularConstraintMotion> ConstraintSwing2Motion;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Settings|PhysicsConstraint|AngularLimit",meta=(AllowPrivateAccess=true))
	float Swing2LimitAngle;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Settings|PhysicsConstraint|AngularLimit",meta=(AllowPrivateAccess=true))
	bool AngularBreakable = false;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Settings|PhysicsConstraint|AngularLimit",meta=(AllowPrivateAccess=true))
	float AngularBreakThreshold = 0;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Settings|PhysicsConstraint|LinearLimit",meta=(AllowPrivateAccess=true))
	TEnumAsByte<ELinearConstraintMotion> XLinearConstraintMotionLimit;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Settings|PhysicsConstraint|LinearLimit",meta=(AllowPrivateAccess=true))
	float XLimitSize;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Settings|PhysicsConstraint|LinearLimit",meta=(AllowPrivateAccess=true))
	TEnumAsByte<ELinearConstraintMotion> YLinearConstraintMotionLimit;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Settings|PhysicsConstraint|LinearLimit",meta=(AllowPrivateAccess=true))
	float YLimitSize;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Settings|PhysicsConstraint|LinearLimit",meta=(AllowPrivateAccess=true))
	TEnumAsByte<ELinearConstraintMotion> ZLinearConstraintMotionLimit;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Settings|PhysicsConstraint|LinearLimit",meta=(AllowPrivateAccess=true))
	float ZLimitSize;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Settings|PhysicsConstraint|AngularMotor",meta=(AllowPrivateAccess=true))
	TEnumAsByte<EAngularDriveMode::Type> AngularDriveMode;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Settings|PhysicsConstraint|AngularMotor",meta=(AllowPrivateAccess=true))
	FVector TargetVelocity;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Settings|PhysicsConstraint|AngularMotor",meta=(AllowPrivateAccess=true))
	bool EnableSwingDrive;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Settings|PhysicsConstraint|AngularMotor",meta=(AllowPrivateAccess=true))
	bool EnableTwistDrive;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Settings|PhysicsConstraint|AngularMotor",meta=(AllowPrivateAccess=true))
	bool EnableSlerpDrive;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Settings|PhysicsConstraint|AngularMotor",meta=(AllowPrivateAccess=true))
	float PositionStrength;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Settings|PhysicsConstraint|AngularMotor",meta=(AllowPrivateAccess=true))
	float VelocityStrength;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Settings|PhysicsConstraint|AngularMotor",meta=(AllowPrivateAccess=true))
	float MaxForce;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Settings|PhysicsConstraint|ConstraintBehaviour",meta=(AllowPrivateAccess=true))
	bool EnableMassConditioning = true;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Settings|PhysicsConstraint|ConstraintBehaviour",meta=(AllowPrivateAccess=true))
	bool ProjectionEnabled = true;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Settings|PhysicsConstraint|MiddleContraint",meta=(AllowPrivateAccess=true))
	bool MiddleIsSoftLimit = true;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Settings|PhysicsConstraint|MiddleContraint",meta=(AllowPrivateAccess=true))
	float MiddleStifness = 100;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Settings|PhysicsConstraint|MiddleContraint",meta=(AllowPrivateAccess=true))
	float MiddleLimitSize = 180;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Settings|PhysicsConstraint|MiddleContraint",meta=(AllowPrivateAccess=true))
	TEnumAsByte<ELinearConstraintMotion> MiddleLimitLinearConstraintMotion = LCM_Limited;

protected:
	virtual void OnConstruction(const FTransform& Transform) override;
	void CustomDestroyConstructedComponents();

private:
	UPhysicsConstraintComponent* CreatePhysicsConstraint(UPrimitiveComponent* FirstComponent, UPrimitiveComponent* SecondComponent, FVector Location, bool
	                             IsRelativeLocation = true);
};
