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
	if (!Reset)
		return;

	GEngine->AddOnScreenDebugMessage(-1,2,FColor::Red,"Construct");

	DestroyConstructedComponents();
	CustomDestroyConstructedComponents();

	float LengthStaticMesh = (StaticMesh != nullptr) ? (StaticMesh->GetBounds().GetBox().GetSize() * Scale).X : 0.0f;

	FVector LastPosition = FVector::ZeroVector;

	for (int i = 0; i < NumberOfLinks; i++)
	{
		UStaticMeshComponent* StaticMeshComponent = NewObject<UStaticMeshComponent>(this);
		StaticMeshComponent->AttachToComponent(CustomRootComponent, FAttachmentTransformRules::KeepRelativeTransform);
		StaticMeshComponent->SetWorldScale3D(Scale);

		if (StaticMesh != nullptr)
			StaticMeshComponent->SetStaticMesh(StaticMesh);

		if (i == 0 || (i + 1) % 2 != 0)
			StaticMeshComponent->SetRelativeRotation(FRotator(0, 0, -90));

		if (i != 0)
		{
			FVector NewPosition = LastPosition + FVector(LengthStaticMesh + OffsetStaticMesh, 0, 0);
			StaticMeshComponent->SetRelativeLocation(NewPosition);
			LastPosition = NewPosition;
		}

		StaticMeshComponent->SetSimulatePhysics(true);
		StaticMeshComponent->SetLinearDamping(LinearDampling);
		StaticMeshComponent->SetAngularDamping(AngularDampling);
		StaticMeshComponent->SetCollisionProfileName("KnifeCable");
		StaticMeshComponent->SetMassOverrideInKg(NAME_None,Mass);

		StaticMeshComponents.Add(StaticMeshComponent);
		StaticMeshComponent->RegisterComponent();

		if (i > 0)
		{
			CreatePhysicsConstraint(StaticMeshComponents[i - 1], StaticMeshComponents[i],
			                        LastPosition - FVector((LengthStaticMesh + OffsetStaticMesh) - (LengthStaticMesh / 2 + OffsetPhysicConstraint)
			                        ,0,0),true);
		}
	}

	if (HasMiddleConstraint)
	{
		UPhysicsConstraintComponent* MiddlePhysicsConstraint = CreatePhysicsConstraint(StaticMeshComponents[0],StaticMeshComponents[NumberOfLinks - 1],
								GetActorLocation() + GetActorForwardVector() *
								((LengthStaticMesh + OffsetStaticMesh)  * NumberOfLinks/2),false);

		MiddlePhysicsConstraint->SetLinearXLimit(MiddleLimitLinearConstraintMotion,MiddleLimitSize);
		MiddlePhysicsConstraint->ConstraintInstance.SetSoftLinearLimitParams(MiddleIsSoftConstraint,MiddleStifness,MiddleDamping,0,0);
	}
	
	if (AttachStart)
	{
		UStaticMeshComponent* StartStaticMeshComponent = StaticMeshComponents[0];
		StartStaticMeshComponent->SetSimulatePhysics(false);

		if (GetParentComponent() != nullptr)
		{
			USceneComponent* AttachSceneComponent = GetParentComponent()->GetAttachParent();
			GEngine->AddOnScreenDebugMessage(-1,5,FColor::Yellow,AttachSceneComponent->GetName());
			GetRootComponent()->AttachToComponent(AttachSceneComponent, FAttachmentTransformRules::KeepRelativeTransform);
		}
	}

	if (StaticMeshComponents.Num() <= 1)
		return;
	
	if (AttachEnd)
	{
		UStaticMeshComponent* EndStaticMeshComponent = StaticMeshComponents[NumberOfLinks - 1];
		EndStaticMeshComponent->SetSimulatePhysics(false);
		AActor* OwingActor = GetParentActor();

		if (OwingActor == nullptr)
			return;
		
		TArray<UActorComponent*> Components;
		OwingActor->GetComponents(Components);

		for (UActorComponent* Component : Components)
		{
			if (Component->GetName() == AttachEndComponentName)
			{
				if (USceneComponent* SceneComponent = Cast<USceneComponent>(Component))
				{
					if (UPrimitiveComponent* PrimitiveComponent = Cast<UPrimitiveComponent>(Component))
					{
						CreatePhysicsConstraint(PrimitiveComponent,EndStaticMeshComponent,
							SceneComponent->GetComponentLocation(),false);
					}
				}
			}
		}
	}
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

UPhysicsConstraintComponent* AChain::CreatePhysicsConstraint(UPrimitiveComponent* FirstComponent, UPrimitiveComponent* SecondComponent,
                                     FVector Location, bool IsRelativeLocation)
{
	UPhysicsConstraintComponent* PhysicsConstraint = NewObject<UPhysicsConstraintComponent>(this);
	PhysicsConstraint->RegisterComponent();
	PhysicsConstraint->AttachToComponent(CustomRootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	PhysicsConstraint->SetConstrainedComponents(FirstComponent, NAME_None, SecondComponent, NAME_None);
	IsRelativeLocation?PhysicsConstraint->SetRelativeLocation(Location):PhysicsConstraint->SetWorldLocation(Location);
	PhysicsConstraint->SetDisableCollision(true);

	PhysicsConstraint->SetAngularTwistLimit(ConstraintTwistMotion, TwistLimitAngle);
	PhysicsConstraint->SetAngularSwing1Limit(ConstraintSwing1Motion, Swing1LimitAngle);
	PhysicsConstraint->SetAngularSwing2Limit(ConstraintSwing2Motion, Swing2LimitAngle);

	PhysicsConstraint->SetLinearXLimit(XLinearConstraintMotionLimit,XLimitSize);
	PhysicsConstraint->SetLinearYLimit(YLinearConstraintMotionLimit,YLimitSize);
	PhysicsConstraint->SetLinearZLimit(ZLinearConstraintMotionLimit,ZLimitSize);

	PhysicsConstraint->SetProjectionEnabled(ProjectionEnabled);

	PhysicsConstraint->SetAngularDriveMode(AngularDriveMode);
	PhysicsConstraint->SetAngularVelocityTarget(TargetVelocity);
	PhysicsConstraint->SetAngularVelocityDrive(EnableSwingDriveTargetVelocityAngularMotor,EnableTwistDriveTargetVelocityAngularMotor);
	PhysicsConstraint->SetAngularDriveParams(PositionStrength,VelocityStrength,MaxForce);

	PhysicsConstraint->SetAngularOrientationDrive(EnableSwingDriveTargetVelocityAngularMotor,EnableTwistDriveTargetVelocityAngularMotor);
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
	
	PhysicsConstraint->SetAngularBreakable(AngularBreakable,AngularBreakThreshold);
	
	PhysicsConstraint->ConstraintInstance.ProfileInstance.bEnableMassConditioning = EnableMassConditioning;

	PhysicsConstraint->SetLinearXLimit(LimitLinearConstraintMotion,LimitSize);
	PhysicsConstraint->ConstraintInstance.SetSoftLinearLimitParams(SoftConstraint,LinearStifness,LinearDamping,0,0);
	
	PhysicsConstraints.Add(PhysicsConstraint);

	return PhysicsConstraint;
}

void AChain::ActivatePhysics()
{
	SetSimulatePhysics(true);
}

void AChain::DeactivatePhysics()
{
	SetSimulatePhysics(false);
}

void AChain::SetSimulatePhysics(bool IsSimulatePhysics)
{
	for (int i = 1; i < StaticMeshComponents.Num() - 1; i++)
	{
		UStaticMeshComponent* StaticMeshComponent = StaticMeshComponents[i];
		StaticMeshComponent->SetSimulatePhysics(IsSimulatePhysics);
		
		if (!IsSimulatePhysics)
			StaticMeshComponent->AttachToComponent(CustomRootComponent, FAttachmentTransformRules::KeepWorldTransform);
	}
}

void AChain::ActivatePhysicsConstraint()
{
	SetPhysicsConstraint(true);
}

void AChain::DeactivatePhysicsConstraint()
{
	SetPhysicsConstraint(false);
}


void AChain::SetPhysicsConstraint(bool IsSimulatePhysics)
{
	for (int i = 1; i < PhysicsConstraints.Num(); i++)
	{
		UPhysicsConstraintComponent* PhysicsConstraint = PhysicsConstraints[i];
		if (IsSimulatePhysics)
			PhysicsConstraint->Activate();
		else
			PhysicsConstraint->Deactivate();
	}
}

