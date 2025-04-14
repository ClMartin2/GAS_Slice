#include "Chain.h"

AChain::AChain()
{
	PrimaryActorTick.bCanEverTick = true;
	CustomRootComponent = CreateDefaultSubobject<USceneComponent>(FName("RootComponent"));
	RootComponent = CustomRootComponent;

	StaticMeshComponents.Init(nullptr, 0);
}

void AChain::InitializeChain()
{
	if (!Reset)
		return;
	
	DestroyConstructedComponents();
	CustomDestroyConstructedComponents();

	UStaticMeshComponent* StaticMeshComponent = CreateStaticMesh(true, true);
	StaticMeshComponent->SetRelativeRotation(FRotator(0, 0, -90));

	LengthStaticMesh = (StaticMesh != nullptr) ? (StaticMesh->GetBounds().GetBox().GetSize() * Scale).X : 0.0f;
	LastPosition = FVector::ZeroVector;

	for (int i = 1; i < NumberOfLinks; i++)
	{
		StaticMeshComponent = CreateStaticMesh(StartWithPhysic, false);

		if ((i + 1) % 2 != 0)
			StaticMeshComponent->SetRelativeRotation(FRotator(0, 0, -90));

		CreatePhysicsConstraint(StaticMeshComponents[i - 1], StaticMeshComponents[i],
		                        LastPosition - FVector(
			                        (LengthStaticMesh + OffsetStaticMesh) - (LengthStaticMesh / 2 +
				                        OffsetPhysicConstraint.X)
			                        , OffsetPhysicConstraint.Y, OffsetPhysicConstraint.Z), true);
	}

	if (HasMiddleConstraint)
	{
		UPhysicsConstraintComponent* MiddlePhysicsConstraint = CreatePhysicsConstraint(
			StaticMeshComponents[0], StaticMeshComponents[NumberOfLinks - 1],
			GetActorLocation() + GetActorForwardVector() *
			((LengthStaticMesh + OffsetStaticMesh) * NumberOfLinks / 2), false);

		MiddlePhysicsConstraint->SetLinearXLimit(MiddleLimitLinearConstraintMotion, MiddleLimitSize);
		MiddlePhysicsConstraint->ConstraintInstance.SetSoftLinearLimitParams(
			MiddleIsSoftConstraint, MiddleStifness, MiddleDamping, 0, 0);
	}

	if (AttachStart)
	{
		UStaticMeshComponent* StartStaticMeshComponent = StaticMeshComponents[0];
		StartStaticMeshComponent->SetSimulatePhysics(false);

		if (GetParentComponent() != nullptr)
		{
			USceneComponent* AttachSceneComponent = GetParentComponent()->GetAttachParent();
			GetRootComponent()->AttachToComponent(AttachSceneComponent,
			                                      FAttachmentTransformRules::KeepRelativeTransform);
		}
	}

	if (StaticMeshComponents.Num() <= 1)
		return;

	if (AttachEnd)
		AttachEndStaticMesh();
}

void AChain::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	InitializeChain();
}

void AChain::CustomDestroyConstructedComponents()
{
	for (UStaticMeshComponent* Component : StaticMeshComponents)
	{
		if (Component)
		{
			Component->DestroyComponent();
		}
	}
	StaticMeshComponents.Empty();

	for (UPhysicsConstraintComponent* Constraint : PhysicsConstraints)
	{
		if (Constraint)
		{
			Constraint->DestroyComponent();
		}
	}
	PhysicsConstraints.Empty();
}

void AChain::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (AttachEnd && StaticMeshComponents.Num() > 0 && ComponentToAttachEndTo != nullptr)
	{
		StaticMeshComponents[StaticMeshComponents.Num() - 1]->SetWorldLocation(
			ComponentToAttachEndTo->GetComponentLocation());
	}
}

void AChain::BeginPlay()
{
	Super::BeginPlay();
	CurrentIndex = StaticMeshComponents.Num();
}

UStaticMeshComponent* AChain::CreateStaticMesh(bool SimulatePhysic = true, bool FirstMesh = false)
{
	UStaticMeshComponent* StaticMeshComponent = NewObject<UStaticMeshComponent>(this);

	StaticMeshComponent->SetRelativeScale3D(Scale);
	
	if (StaticMesh != nullptr)
		StaticMeshComponent->SetStaticMesh(StaticMesh);

	if (!FirstMesh)
	{
		FVector NewPosition = LastPosition + FVector(LengthStaticMesh + OffsetStaticMesh, 0, 0);
		StaticMeshComponent->SetRelativeLocation(NewPosition);
		LastPosition = NewPosition;
	}

	StaticMeshComponent->SetSimulatePhysics(SimulatePhysic);
	StaticMeshComponent->SetLinearDamping(LinearDampling);
	StaticMeshComponent->SetAngularDamping(AngularDampling);
	StaticMeshComponent->SetCollisionProfileName("KnifeCable");
	StaticMeshComponent->SetMassOverrideInKg(NAME_None, Mass);
	
	StaticMeshComponent->BodyInstance.bLockXRotation = bLockXRotation;
	StaticMeshComponent->BodyInstance.bLockYRotation = bLockYRotation;
	StaticMeshComponent->BodyInstance.bLockZRotation = bLockZRotation;
	StaticMeshComponent->BodyInstance.bOverrideMaxAngularVelocity = bOverrideMaxAngularVelocity;
	StaticMeshComponent->BodyInstance.MaxAngularVelocity = MaxAngularVelocity;

	StaticMeshComponent->CastShadow = StaticMeshCastShadow;
	
	StaticMeshComponent->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
	StaticMeshComponents.Add(StaticMeshComponent);

	StaticMeshComponent->RegisterComponent();
	CurrentIndex++;

	return StaticMeshComponent;
}

void AChain::AttachEndStaticMesh()
{
	UStaticMeshComponent* EndStaticMeshComponent = StaticMeshComponents[StaticMeshComponents.Num() - 1];
	EndStaticMeshComponent->SetSimulatePhysics(false);
}

UPhysicsConstraintComponent* AChain::CreatePhysicsConstraint(UPrimitiveComponent* FirstComponent,
                                                             UPrimitiveComponent* SecondComponent,
                                                             FVector Location, bool IsRelativeLocation)
{
	UPhysicsConstraintComponent* PhysicsConstraint = NewObject<UPhysicsConstraintComponent>(this);
	PhysicsConstraint->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);

	PhysicsConstraint->SetConstrainedComponents(FirstComponent, NAME_None, SecondComponent, NAME_None);

	IsRelativeLocation
		? PhysicsConstraint->SetRelativeLocation(Location)
		: PhysicsConstraint->SetWorldLocation(Location);

	PhysicsConstraint->SetDisableCollision(true);

	PhysicsConstraint->SetAngularTwistLimit(ConstraintTwistMotion, TwistLimitAngle);
	PhysicsConstraint->SetAngularSwing1Limit(ConstraintSwing1Motion, Swing1LimitAngle);
	PhysicsConstraint->SetAngularSwing2Limit(ConstraintSwing2Motion, Swing2LimitAngle);

	PhysicsConstraint->SetLinearXLimit(XLinearConstraintMotionLimit, XLimitSize);
	PhysicsConstraint->SetLinearYLimit(YLinearConstraintMotionLimit, YLimitSize);
	PhysicsConstraint->SetLinearZLimit(ZLinearConstraintMotionLimit, ZLimitSize);

	PhysicsConstraint->SetProjectionEnabled(ProjectionEnabled);

	PhysicsConstraint->SetAngularDriveMode(AngularDriveMode);
	PhysicsConstraint->SetAngularVelocityTarget(TargetVelocity);
	PhysicsConstraint->SetAngularVelocityDrive(_EnableSwingDriveTargetVelocityAngularMotor,
	                                           EnableTwistDriveTargetVelocityAngularMotor);
	PhysicsConstraint->SetAngularDriveParams(PositionStrength, VelocityStrength, MaxForce);

	PhysicsConstraint->SetAngularOrientationDrive(_EnableSwingDriveTargetVelocityAngularMotor,
	                                              EnableTwistDriveTargetVelocityAngularMotor);
	PhysicsConstraint->SetAngularOrientationTarget(TargetOrientation);

	if (AngularDriveMode == EAngularDriveMode::TwistAndSwing)
	{
		PhysicsConstraint->SetAngularVelocityDriveSLERP(false);
		PhysicsConstraint->SetOrientationDriveSLERP(false);
	}
	else
	{
		PhysicsConstraint->SetAngularVelocityDriveSLERP(true);
		PhysicsConstraint->SetOrientationDriveSLERP(true);
	}

	PhysicsConstraint->SetAngularBreakable(AngularBreakable, AngularBreakThreshold);
	PhysicsConstraint->SetLinearBreakable(LinearBreakable, LinearBreakableThreshold);

	PhysicsConstraint->ConstraintInstance.ProfileInstance.bEnableMassConditioning = EnableMassConditioning;
	
	PhysicsConstraint->ConstraintInstance.SetSoftLinearLimitParams(SoftConstraint, LinearStifness, LinearDamping, 0, 0);
	PhysicsConstraint->ConstraintInstance.SetSoftSwingLimitParams(SoftSwingLimit, SoftSwingStifness, SoftSwingDamping, 0, 0);

	PhysicsConstraint->ConstraintInstance.SetShockPropagationParams(ShockPropagationEnabled, ShockPropagationAlpha);

	PhysicsConstraint->SetAngularPlasticity(AngularPlasticity, AngularPlasticityThreshold);

	PhysicsConstraint->OnConstraintBroken.AddDynamic(this, &AChain::OnConstraintBroken);
	
	PhysicsConstraint->SetProjectionParams(ProjectionLinearAlpha, ProjectionAngularAlpha,
		ProjectionLinearTolerance, ProjectionAngularTolerance);
	
	PhysicsConstraints.Add(PhysicsConstraint);
	PhysicsConstraint->RegisterComponent();

	return PhysicsConstraint;
}

void AChain::OnConstraintBroken(int32 ConstraintIndex)
{
	OnChainBreakDelegate.Execute();
}

void AChain::SetSimulatePhysics(bool IsSimulatePhysics)
{
	if (StaticMeshComponents.IsEmpty())
		return;

	for (int i = 1; i < StaticMeshComponents.Num() - 1; i++)
	{
		UStaticMeshComponent* StaticMeshComponent = StaticMeshComponents[i];
		StaticMeshComponent->SetSimulatePhysics(IsSimulatePhysics);

		if (!IsSimulatePhysics)
			StaticMeshComponent->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::KeepWorldTransform);
	}
}

void AChain::SetAngularBreakable(bool _AngularBreakable, bool _LinearBreakable)
{
	for (int i = 3; i < PhysicsConstraints.Num() - 1; i++)
	{
		UPhysicsConstraintComponent* PhysicsConstraintComponent = PhysicsConstraints[i];
		
		PhysicsConstraintComponent->SetAngularBreakable(_AngularBreakable, AngularBreakThreshold);
		PhysicsConstraintComponent->SetLinearBreakable(_LinearBreakable, LinearBreakableThreshold);
	}
}

