#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PhysicsCore.h"
#include "PhysicsEngine/ConstraintDrives.h"
#include "Chain.generated.h"

class UPhysicsConstraintComponent;

DECLARE_DELEGATE(FOnChainBreak);

UCLASS()
class GAS_SLICE_API AChain : public AActor
{
	GENERATED_BODY()
	
public:	
	AChain();

public:
	FOnChainBreak OnChainBreakDelegate;

private:
#pragma region Settings

	UPROPERTY(EditAnywhere,Category="Settings",meta=(AllowPrivateAccess=true))
	bool Reset = false;

	UPROPERTY(EditAnywhere,Category="Settings",meta=(AllowPrivateAccess=true))
	int NumberOfLinks = 10;

	UPROPERTY(EditAnywhere,Category="Settings",meta=(AllowPrivateAccess=true))
	TArray<UStaticMeshComponent*> StaticMeshComponents;

	UPROPERTY(EditAnywhere,Category="Settings",meta=(AllowPrivateAccess=true))
	TArray<UPhysicsConstraintComponent*> PhysicsConstraints;

	UPROPERTY(EditAnywhere,Category="Settings",meta=(AllowPrivateAccess=true))
	USceneComponent* CustomRootComponent;

	UPROPERTY(EditAnywhere,Category="Settings",meta=(AllowPrivateAccess=true))
	float OffsetStaticMesh;

	UPROPERTY(EditAnywhere,Category="Settings",meta=(AllowPrivateAccess=true))
	FVector OffsetPhysicConstraint;

	UPROPERTY(EditAnywhere,Category="Settings",meta=(AllowPrivateAccess=true))
	FVector Scale = FVector::One();
	
	UPROPERTY(EditAnywhere,Category="Settings",meta=(AllowPrivateAccess=true))
	UStaticMesh* StaticMesh;

	UPROPERTY(EditAnywhere,Category="Settings",meta=(AllowPrivateAccess=true))
	bool AttachStart = true;

	UPROPERTY(EditAnywhere,Category="Settings",meta=(AllowPrivateAccess=true))
	bool AttachEnd = false;

	UPROPERTY(EditAnywhere,Category="Settings",meta=(AllowPrivateAccess=true))
	FString AttachEndComponentName;

	UPROPERTY(EditAnywhere,Category="Settings",meta=(AllowPrivateAccess=true))
	bool StaticMeshCastShadow = false;
	
#pragma endregion Settings

#pragma region Settings|Physics

	UPROPERTY(EditAnywhere,Category="Settings|Physics",meta=(AllowPrivateAccess=true))
	float LinearDampling = 0.01f;

	UPROPERTY(EditAnywhere,Category="Settings|Physics",meta=(AllowPrivateAccess=true))
	float AngularDampling = 0;
	
	UPROPERTY(EditAnywhere,Category="Settings|Physics",meta=(AllowPrivateAccess=true))
	bool bLockXRotation = true;

	UPROPERTY(EditAnywhere,Category="Settings|Physics",meta=(AllowPrivateAccess=true))
	bool bLockYRotation = true;

	UPROPERTY(EditAnywhere,Category="Settings|Physics",meta=(AllowPrivateAccess=true))
	bool bLockZRotation = true;

	UPROPERTY(EditAnywhere,Category="Settings|Physics",meta=(AllowPrivateAccess=true))
	bool StartWithPhysic = true;
	
	UPROPERTY(EditAnywhere,Category="Settings|Physics",meta=(AllowPrivateAccess=true))
	float Mass = 1;

	UPROPERTY(EditAnywhere,Category="Settings|Physics",meta=(AllowPrivateAccess=true))
	bool bOverrideMaxAngularVelocity = false;

	UPROPERTY(EditAnywhere,Category="Settings|Physics",meta=(AllowPrivateAccess=true))
	float MaxAngularVelocity = 100;

#pragma endregion Settings Physics

#pragma region Settings PhysicsConstraint AngularLimit

	UPROPERTY(EditAnywhere,Category="Settings|PhysicsConstraint|AngularLimit",meta=(AllowPrivateAccess=true))
	TEnumAsByte<EAngularConstraintMotion> ConstraintTwistMotion;

	UPROPERTY(EditAnywhere,Category="Settings|PhysicsConstraint|AngularLimit",meta=(AllowPrivateAccess=true))
	float TwistLimitAngle;

	UPROPERTY(EditAnywhere,Category="Settings|PhysicsConstraint|AngularLimit",meta=(AllowPrivateAccess=true))
	TEnumAsByte<EAngularConstraintMotion> ConstraintSwing1Motion;

	UPROPERTY(EditAnywhere,Category="Settings|PhysicsConstraint|AngularLimit",meta=(AllowPrivateAccess=true))
	float Swing1LimitAngle;

	UPROPERTY(EditAnywhere,Category="Settings|PhysicsConstraint|AngularLimit",meta=(AllowPrivateAccess=true))
	TEnumAsByte<EAngularConstraintMotion> ConstraintSwing2Motion;

	UPROPERTY(EditAnywhere,Category="Settings|PhysicsConstraint|AngularLimit",meta=(AllowPrivateAccess=true))
	float Swing2LimitAngle;
	
	UPROPERTY(EditAnywhere,Category="Settings|PhysicsConstraint|AngularLimit",meta=(AllowPrivateAccess=true))
	bool AngularBreakable = false;

	UPROPERTY(EditAnywhere,Category="Settings|PhysicsConstraint|AngularLimit",meta=(AllowPrivateAccess=true))
	float AngularBreakThreshold = 0;

	UPROPERTY(EditAnywhere,Category="Settings|PhysicsConstraint|AngularLimit",meta=(AllowPrivateAccess=true))
	bool SoftSwingLimit = true;

	UPROPERTY(EditAnywhere,Category="Settings|PhysicsConstraint|AngularLimit",meta=(AllowPrivateAccess=true))
	float SoftSwingStifness = 50;

	UPROPERTY(EditAnywhere,Category="Settings|PhysicsConstraint|AngularLimit",meta=(AllowPrivateAccess=true))
	float SoftSwingDamping = 3;

	UPROPERTY(EditAnywhere,Category="Settings|PhysicsConstraint|AngularLimit",meta=(AllowPrivateAccess=true))
	bool AngularPlasticity = false;

	UPROPERTY(EditAnywhere,Category="Settings|PhysicsConstraint|AngularLimit",meta=(AllowPrivateAccess=true))
	float AngularPlasticityThreshold;


#pragma endregion Settings PhysicsConstraint AngularLimit

#pragma region Settings PhysicsConstraint LinearLimit

	UPROPERTY(EditAnywhere,Category="Settings|PhysicsConstraint|LinearLimit",meta=(AllowPrivateAccess=true))
	TEnumAsByte<ELinearConstraintMotion> XLinearConstraintMotionLimit;

	UPROPERTY(EditAnywhere,Category="Settings|PhysicsConstraint|LinearLimit",meta=(AllowPrivateAccess=true))
	float XLimitSize;

	UPROPERTY(EditAnywhere,Category="Settings|PhysicsConstraint|LinearLimit",meta=(AllowPrivateAccess=true))
	TEnumAsByte<ELinearConstraintMotion> YLinearConstraintMotionLimit;

	UPROPERTY(EditAnywhere,Category="Settings|PhysicsConstraint|LinearLimit",meta=(AllowPrivateAccess=true))
	float YLimitSize;

	UPROPERTY(EditAnywhere,Category="Settings|PhysicsConstraint|LinearLimit",meta=(AllowPrivateAccess=true))
	TEnumAsByte<ELinearConstraintMotion> ZLinearConstraintMotionLimit;
	
	UPROPERTY(EditAnywhere,Category="Settings|PhysicsConstraint|LinearLimit",meta=(AllowPrivateAccess=true))
	float ZLimitSize;
	
	UPROPERTY(EditAnywhere,Category="Settings|PhysicsConstraint|LinearLimit",meta=(AllowPrivateAccess=true))
	bool SoftConstraint = true;

	UPROPERTY(EditAnywhere,Category="Settings|PhysicsConstraint|LinearLimit",meta=(AllowPrivateAccess=true))
	float LinearStifness = 100;
	
	UPROPERTY(EditAnywhere,Category="Settings|PhysicsConstraint|LinearLimit",meta=(AllowPrivateAccess=true))
	float LinearDamping = 0;

	UPROPERTY(EditAnywhere,Category="Settings|PhysicsConstraint|LinearLimit",meta=(AllowPrivateAccess=true))
	bool LinearBreakable = false;

	UPROPERTY(EditAnywhere,Category="Settings|PhysicsConstraint|LinearLimit",meta=(AllowPrivateAccess=true))
	float LinearBreakableThreshold = 0;

#pragma endregion Settings PhysicsConstraint LinearLimit

#pragma region Settings PhysicsConstraint AngularMotor

	UPROPERTY(EditAnywhere,Category="Settings|PhysicsConstraint|AngularMotor",meta=(AllowPrivateAccess=true))
	TEnumAsByte<EAngularDriveMode::Type> AngularDriveMode;

	UPROPERTY(EditAnywhere,Category="Settings|PhysicsConstraint|AngularMotor",meta=(AllowPrivateAccess=true))
	FVector TargetVelocity;

	UPROPERTY(EditAnywhere,Category="Settings|PhysicsConstraint|AngularMotor",meta=(AllowPrivateAccess=true))
	FRotator TargetOrientation;

	UPROPERTY(EditAnywhere,Category="Settings|PhysicsConstraint|AngularMotor",meta=(AllowPrivateAccess=true))
	bool _EnableSwingDriveTargetVelocityAngularMotor = true;

	UPROPERTY(EditAnywhere,Category="Settings|PhysicsConstraint|AngularMotor",meta=(AllowPrivateAccess=true))
	bool EnableTwistDriveTargetVelocityAngularMotor;

	UPROPERTY(EditAnywhere,Category="Settings|PhysicsConstraint|AngularMotor",meta=(AllowPrivateAccess=true))
	bool EnableSwingDriveTargetOrientationAngularMotor;

	UPROPERTY(EditAnywhere,Category="Settings|PhysicsConstraint|AngularMotor",meta=(AllowPrivateAccess=true))
	bool EnableTwistDriveTargetOrientationAngularMotor;

	UPROPERTY(EditAnywhere,Category="Settings|PhysicsConstraint|AngularMotor",meta=(AllowPrivateAccess=true))
	float PositionStrength;

	UPROPERTY(EditAnywhere,Category="Settings|PhysicsConstraint|AngularMotor",meta=(AllowPrivateAccess=true))
	float VelocityStrength;

	UPROPERTY(EditAnywhere,Category="Settings|PhysicsConstraint|AngularMotor",meta=(AllowPrivateAccess=true))
	float MaxForce;

#pragma endregion Settings PhysicsConstraint AngularMotor

#pragma region Settings PhysicsConstraint ConstraintBehaviour
	
	UPROPERTY(EditAnywhere,Category="Settings|PhysicsConstraint|ConstraintBehaviour",meta=(AllowPrivateAccess=true))
	bool EnableMassConditioning = true;

	UPROPERTY(EditAnywhere,Category="Settings|PhysicsConstraint|ConstraintBehaviour",meta=(AllowPrivateAccess=true))
	bool ProjectionEnabled = true;

	UPROPERTY(EditAnywhere,Category="Settings|PhysicsConstraint|ConstraintBehaviour",meta=(AllowPrivateAccess=true))
	float ProjectionLinearTolerance;

	UPROPERTY(EditAnywhere,Category="Settings|PhysicsConstraint|ConstraintBehaviour",meta=(AllowPrivateAccess=true))
	float ProjectionAngularTolerance;

	UPROPERTY(EditAnywhere,Category="Settings|PhysicsConstraint|ConstraintBehaviour",meta=(AllowPrivateAccess=true,
		ClampMin = "0.0", ClampMax = "1.0", UIMin = "0.0", UIMax = "1.0"))
	float ProjectionLinearAlpha;
	
	UPROPERTY(EditAnywhere,Category="Settings|PhysicsConstraint|ConstraintBehaviour",meta=(AllowPrivateAccess=true,
		ClampMin = "0.0", ClampMax = "1.0", UIMin = "0.0", UIMax = "1.0"))
	float ProjectionAngularAlpha;
	
	UPROPERTY(EditAnywhere,Category="Settings|PhysicsConstraint|ConstraintBehaviour",meta=(AllowPrivateAccess=true))
	bool ShockPropagationEnabled = false;

	UPROPERTY(EditAnywhere,Category="Settings|PhysicsConstraint|ConstraintBehaviour",meta=(AllowPrivateAccess=true))
	float ShockPropagationAlpha = 0.3;
	
#pragma endregion Settings PhysicsConstraint ConstraintBehaviour

#pragma region Settings PhysicsConstraint MiddleConstraint

	UPROPERTY(EditAnywhere,Category="Settings|PhysicsConstraint|MiddleContraint",meta=(AllowPrivateAccess=true))
	bool HasMiddleConstraint = false;

	UPROPERTY(EditAnywhere,Category="Settings|PhysicsConstraint|MiddleContraint",meta=(AllowPrivateAccess=true))
	bool MiddleIsSoftConstraint = true;

	UPROPERTY(EditAnywhere,Category="Settings|PhysicsConstraint|MiddleContraint",meta=(AllowPrivateAccess=true))
	float MiddleStifness = 100;
	
	UPROPERTY(EditAnywhere,Category="Settings|PhysicsConstraint|MiddleContraint",meta=(AllowPrivateAccess=true))
	float MiddleDamping = 0;

	UPROPERTY(EditAnywhere,Category="Settings|PhysicsConstraint|MiddleContraint",meta=(AllowPrivateAccess=true))
	float MiddleLimitSize = 180;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Settings|PhysicsConstraint|MiddleContraint",meta=(AllowPrivateAccess=true))
	TEnumAsByte<ELinearConstraintMotion> MiddleLimitLinearConstraintMotion = LCM_Limited;
#pragma endregion Settings PhysicsConstraint MiddleConstraint
	
	UPROPERTY(EditAnywhere,Category="Debug",meta=(AllowPrivateAccess=true))
	FVector LastPosition = FVector::Zero();
	
	UPROPERTY(EditAnywhere,Category="Debug",meta=(AllowPrivateAccess=true))
	float LengthStaticMesh = 0;
	
	UPROPERTY(EditAnywhere,Category="Debug",meta=(AllowPrivateAccess=true))
	int CurrentIndex = 0;
	
	TArray<UStaticMeshComponent*> DynamicStaticMeshComponents;
	TArray<UPhysicsConstraintComponent*> DynamicPhysicsConstraintComponent;
	UPhysicsConstraintComponent* AttachEndPhysicsConstraint;
	USceneComponent* ComponentToAttachEndTo;
	TMap<UStaticMeshComponent*, UPhysicsConstraintComponent*> DynamicMeshToPhysicsConstraint;
	
public :
	UFUNCTION(BlueprintCallable,Category="Physics",meta=(AllowPrivateAccess=true))
	void SetSimulatePhysics(bool IsSimulatePhysics);

	UFUNCTION(BlueprintCallable,Category="Physics",meta=(AllowPrivateAccess=true))
	UStaticMeshComponent* AddDynamicMesh(bool SimulatePhysics, bool _AngularBreakable = false, bool _LinearBreakable = false);
	
	UFUNCTION(BlueprintCallable,Category="Physics",meta=(AllowPrivateAccess=true))
	void DestroyDynamicMesh(UStaticMeshComponent* StaticMeshComponent,bool ClearDynamicArray = true);

	void DestroyAllDynamicMeshes();
	FVector GetLastPosition() const {return LastPosition;}
	float GetLengthMesh() const {return LengthStaticMesh;}
	float GetOffsetBetweenMesh() const {return OffsetStaticMesh;}
	void SetAngularBreakable(bool _AngularBreakable, bool _LinearBreakable);
	void SetComponentToAttachEnd(USceneComponent* EndComponentToAttach){ComponentToAttachEndTo = EndComponentToAttach;}
	TArray<UStaticMeshComponent*> GetStaticMeshComponents() const {return StaticMeshComponents;}
	void InitializeChain();
	void OnConstruction(const FTransform& Transform);

protected:
	void CustomDestroyConstructedComponents();
	virtual void Tick(float DeltaSeconds) override;
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable,Category="Chain",meta=(AllowPrivateAccess=true))
	TArray<UStaticMeshComponent*> GetInstantiedDyanmicMeshes(){return DynamicStaticMeshComponents;}

private:
	UFUNCTION()
	void OnConstraintBroken(int32 ConstraintIndex);
	UPhysicsConstraintComponent* CreatePhysicsConstraint(UPrimitiveComponent* FirstComponent, UPrimitiveComponent* SecondComponent, FVector Location, bool
	                                                     IsRelativeLocation = true);
	UStaticMeshComponent* CreateStaticMesh(bool SimulatePhysic, bool FirstMesh);
	void AttachEndStaticMesh();
};
