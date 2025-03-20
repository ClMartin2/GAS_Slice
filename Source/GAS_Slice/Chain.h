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
#pragma region Settings

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
	
#pragma endregion Settings

#pragma region Settings|Physics

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Settings|Physics",meta=(AllowPrivateAccess=true))
	bool bLockXRotation = true;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Settings|Physics",meta=(AllowPrivateAccess=true))
	bool bLockYRotation = true;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Settings|Physics",meta=(AllowPrivateAccess=true))
	bool bLockZRotation = true;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Settings|Physics",meta=(AllowPrivateAccess=true))
	bool StartWithPhysic = true;

#pragma endregion Settings Physics

#pragma region Settings PhysicsConstraint AngularLimit

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

#pragma endregion Settings PhysicsConstraint AngularLimit

#pragma region Settings PhysicsConstraint LinearLimit

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
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Settings|PhysicsConstraint|LinearLimit",meta=(AllowPrivateAccess=true))
	bool SoftConstraint = true;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Settings|PhysicsConstraint|LinearLimit",meta=(AllowPrivateAccess=true))
	float LinearStifness = 100;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Settings|PhysicsConstraint|LinearLimit",meta=(AllowPrivateAccess=true))
	float LinearDamping = 0;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Settings|PhysicsConstraint|LinearLimit",meta=(AllowPrivateAccess=true))
	float LimitSize = 180;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Settings|PhysicsConstraint|LinearLimit",meta=(AllowPrivateAccess=true))
	TEnumAsByte<ELinearConstraintMotion> LimitLinearConstraintMotion = LCM_Locked;

#pragma endregion Settings PhysicsConstraint LinearLimit

#pragma region Settings PhysicsConstraint AngularMotor

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Settings|PhysicsConstraint|AngularMotor",meta=(AllowPrivateAccess=true))
	TEnumAsByte<EAngularDriveMode::Type> AngularDriveMode;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Settings|PhysicsConstraint|AngularMotor",meta=(AllowPrivateAccess=true))
	FVector TargetVelocity;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Settings|PhysicsConstraint|AngularMotor",meta=(AllowPrivateAccess=true))
	FRotator TargetOrientation;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Settings|PhysicsConstraint|AngularMotor",meta=(AllowPrivateAccess=true))
	bool EnableSwingDriveTargetVelocityAngularMotor;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Settings|PhysicsConstraint|AngularMotor",meta=(AllowPrivateAccess=true))
	bool EnableTwistDriveTargetVelocityAngularMotor;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Settings|PhysicsConstraint|AngularMotor",meta=(AllowPrivateAccess=true))
	bool EnableSwingDriveTargetOrientationAngularMotor;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Settings|PhysicsConstraint|AngularMotor",meta=(AllowPrivateAccess=true))
	bool EnableTwistDriveTargetOrientationAngularMotor;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Settings|PhysicsConstraint|AngularMotor",meta=(AllowPrivateAccess=true))
	float PositionStrength;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Settings|PhysicsConstraint|AngularMotor",meta=(AllowPrivateAccess=true))
	float VelocityStrength;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Settings|PhysicsConstraint|AngularMotor",meta=(AllowPrivateAccess=true))
	float MaxForce;

#pragma endregion Settings PhysicsConstraint AngularMotor

#pragma region Settings PhysicsConstraint ConstraintBehaviour
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Settings|PhysicsConstraint|ConstraintBehaviour",meta=(AllowPrivateAccess=true))
	bool EnableMassConditioning = true;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Settings|PhysicsConstraint|ConstraintBehaviour",meta=(AllowPrivateAccess=true))
	bool ProjectionEnabled = true;
	
#pragma endregion Settings PhysicsConstraint ConstraintBehaviour

#pragma region Settings PhysicsConstraint MiddleConstraint

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Settings|PhysicsConstraint|MiddleContraint",meta=(AllowPrivateAccess=true))
	bool HasMiddleConstraint = false;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Settings|PhysicsConstraint|MiddleContraint",meta=(AllowPrivateAccess=true))
	bool MiddleIsSoftConstraint = true;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Settings|PhysicsConstraint|MiddleContraint",meta=(AllowPrivateAccess=true))
	float MiddleStifness = 100;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Settings|PhysicsConstraint|MiddleContraint",meta=(AllowPrivateAccess=true))
	float MiddleDamping = 0;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Settings|PhysicsConstraint|MiddleContraint",meta=(AllowPrivateAccess=true))
	float MiddleLimitSize = 180;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Settings|PhysicsConstraint|MiddleContraint",meta=(AllowPrivateAccess=true))
	TEnumAsByte<ELinearConstraintMotion> MiddleLimitLinearConstraintMotion = LCM_Limited;
#pragma endregion Settings PhysicsConstraint MiddleConstraint

	FVector LastPosition = FVector::Zero();
	float LengthStaticMesh = 0;
	int currentIndex = 0;
	TArray<UStaticMeshComponent*> DynamicStaticMeshComponents;
	UPhysicsConstraintComponent* AttachEndPhysicsConstraint;
	USceneComponent* ComponentToAttachEndTo;
	UStaticMeshComponent* DebugStaticMeshComponent;
	FVector LastMeshPosition = FVector::ZeroVector;
	
public :
	UFUNCTION(BlueprintCallable,Category="Physics",meta=(AllowPrivateAccess=true))
	void SetSimulatePhysics(bool IsSimulatePhysics);

	UFUNCTION(BlueprintCallable,Category="Physics",meta=(AllowPrivateAccess=true))
	UStaticMeshComponent* AddDynamicMesh(bool SimulatePhysics);

	UFUNCTION(BlueprintCallable,Category="Vector",meta=(AllowPrivateAccess=true))
	FVector GetLastPosition() const {return LastPosition;}

	UFUNCTION(BlueprintCallable,Category="Vector",meta=(AllowPrivateAccess=true))
	float GetLengthStaticMesh() const {return LengthStaticMesh;}

	void SetComponentToAttachEnd(USceneComponent* EndComponentToAttach){ComponentToAttachEndTo = EndComponentToAttach;}

	TArray<UStaticMeshComponent*> GetStaticMeshComponents() const {return StaticMeshComponents;}

protected:
	virtual void OnConstruction(const FTransform& Transform) override;
	void CustomDestroyConstructedComponents();
	virtual void Tick(float DeltaSeconds) override;

	UFUNCTION(BlueprintCallable,Category="Chain",meta=(AllowPrivateAccess=true))
	TArray<UStaticMeshComponent*> GetInstantiedDyanmicMeshes(){return DynamicStaticMeshComponents;}

private:
	UPhysicsConstraintComponent* CreatePhysicsConstraint(UPrimitiveComponent* FirstComponent, UPrimitiveComponent* SecondComponent, FVector Location, bool
	                             IsRelativeLocation = true);
	UStaticMeshComponent* CreateStaticMesh(bool SimulatePhysic, bool FirstMesh);
	void AttachEndStaticMesh(USceneComponent* ComponentToAttach);
};
