#include "Chain.h"

#include "LandscapeGizmoActiveActor.h"
#include "PhysicsEngine/PhysicsConstraintComponent.h"

AChain::AChain()
{
	PrimaryActorTick.bCanEverTick = true;
	CustomRootComponent = CreateDefaultSubobject<USceneComponent>(FName("RootComponent"));
	RootComponent = CustomRootComponent;

	StaticMeshComponents.Init(nullptr, 0);
}

void AChain::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	if (!Reset)
		return;

	GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Red, "Construct");

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
				                        OffsetPhysicConstraint)
			                        , 0, 0), true);
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
			GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Yellow, AttachSceneComponent->GetName());
			GetRootComponent()->AttachToComponent(AttachSceneComponent,
			                                      FAttachmentTransformRules::KeepRelativeTransform);
		}
	}

	if (StaticMeshComponents.Num() <= 1)
		return;

	if (AttachEnd)
		AttachEndStaticMesh();
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

	for (UStaticMeshComponent* Component : DynamicStaticMeshComponents)
	{
		GEngine->AddOnScreenDebugMessage(-1,0.1,FColor::Yellow,"Previous Mesh Simulate Physic: "
			+ LexToString(Component->IsSimulatingPhysics()));

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
	PhysicsConstraint->SetAngularVelocityDrive(EnableSwingDriveTargetVelocityAngularMotor,
	                                           EnableTwistDriveTargetVelocityAngularMotor);
	PhysicsConstraint->SetAngularDriveParams(PositionStrength, VelocityStrength, MaxForce);

	PhysicsConstraint->SetAngularOrientationDrive(EnableSwingDriveTargetVelocityAngularMotor,
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

	PhysicsConstraint->ConstraintInstance.ProfileInstance.bEnableMassConditioning = EnableMassConditioning;

	PhysicsConstraint->SetLinearXLimit(LimitLinearConstraintMotion, LimitSize);
	PhysicsConstraint->ConstraintInstance.SetSoftLinearLimitParams(SoftConstraint, LinearStifness, LinearDamping, 0, 0);

	PhysicsConstraint->ConstraintInstance.SetShockPropagationParams(ShockPropagationEnabled, ShockPropagationAlpha);

	PhysicsConstraints.Add(PhysicsConstraint);
	PhysicsConstraint->RegisterComponent();

	return PhysicsConstraint;
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
			StaticMeshComponent->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
	}
}

UStaticMeshComponent* AChain::AddDynamicMesh(bool SimulatePhysics)
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

	GEngine->AddOnScreenDebugMessage(-1,2,FColor::Yellow,"Previous Mesh Simulate Physic: " + LexToString(PreviousLastMesh->IsSimulatingPhysics()));
	UE_LOG(LogTemp, Warning, TEXT("Previous Mesh Simulate Physic: %s"), *LexToString(PreviousLastMesh->IsSimulatingPhysics()));
	
	if (CurrentIndex % 2 != 0)
		StaticMeshComponent->SetRelativeRotation(FRotator(0, 0, -90));

	if (CurrentIndex < 2)
		return nullptr;

	FVector NewLocation = LastPosition - FVector(
		(LengthStaticMesh + OffsetStaticMesh) - (LengthStaticMesh / 2 + OffsetPhysicConstraint), 0, 0);

	UPhysicsConstraintComponent* PhysicsConstraintComponent = CreatePhysicsConstraint(
		StaticMeshComponents[CurrentIndex - 2],
		StaticMeshComponents[CurrentIndex - 1], NewLocation, true);

	DrawDebugDirectionalArrow(GetWorld(), NewLocation, NewLocation + FVector::UpVector * 100000000000000, 10,
	                          FColor::Red,
	                          true, -1,
	                          0, 2);

	GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Blue, "Add Dynamic Mesh");

	DynamicStaticMeshComponents.Add(StaticMeshComponent);
	DynamicPhysicsConstraintComponent.Add(PhysicsConstraintComponent);
	DynamicMeshToPhysicsConstraint.Add(StaticMeshComponent, PhysicsConstraintComponent);

	return StaticMeshComponent;
}

void AChain::DestroyDynamicMesh(UStaticMeshComponent* StaticMeshComponent)
{
	UPhysicsConstraintComponent* PhysicsConstraintComponent = DynamicMeshToPhysicsConstraint[StaticMeshComponent];

	StaticMeshComponents.Remove(StaticMeshComponent);
	DynamicStaticMeshComponents.Remove(StaticMeshComponent);
	PhysicsConstraints.Remove(PhysicsConstraintComponent);

	StaticMeshComponent->DestroyComponent();
	PhysicsConstraintComponent->DestroyComponent();
	CurrentIndex = StaticMeshComponents.Num();
	LastPosition = StaticMeshComponents.Num() * FVector(LengthStaticMesh + OffsetStaticMesh, 0, 0);

	//Correction when set new end mesh to attach this to the root and correct is rotation
	UStaticMeshComponent* NewLastMesh = StaticMeshComponents[StaticMeshComponents.Num() - 1];

	if (AttachEnd )
	{
		AttachEndStaticMesh();
		NewLastMesh->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::KeepWorldTransform);
		if (CurrentIndex % 2 != 0)
			NewLastMesh->SetRelativeRotation(FRotator(0, 0, -90));
	}

	GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Blue, "Remove Dynamic Mesh");
}
