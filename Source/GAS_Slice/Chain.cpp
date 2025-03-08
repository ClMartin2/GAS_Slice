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

		if (i == 0 || (i + 1) % 2 != 0)
			StaticMeshComponent->SetRelativeRotation(FRotator(0, 0, -90));

		if (i != 0)
		{
			FVector NewPosition = LastPosition + FVector(LengthStaticMesh + OffsetStaticMesh, 0, 0);
			StaticMeshComponent->SetRelativeLocation(NewPosition);
			LastPosition = NewPosition;

			StaticMeshComponent->SetSimulatePhysics(true);
		}

		StaticMeshComponent->SetLinearDamping(LinearDampling);
		StaticMeshComponent->SetAngularDamping(AngularDampling);
		StaticMeshComponent->SetCollisionProfileName("KnifeCable");

		ChainLinks.Add(StaticMeshComponent);
		StaticMeshComponents.Add(StaticMeshComponent);

		StaticMeshComponent->RegisterComponent();

		if (i > 0)
		{
			CreatePhysicsConstraint(ChainLinks[i - 1], ChainLinks[i],
			                        LastPosition - FVector((LengthStaticMesh + OffsetStaticMesh) - (LengthStaticMesh / 2 + OffsetPhysicConstraint)
			                        ,0,0),true);
		}
	}

	if (AttachStart)
	{
		UStaticMeshComponent* StartStaticMeshComponent = StaticMeshComponents[0];
		StartStaticMeshComponent->SetSimulatePhysics(false);

		if (GetParentComponent() != nullptr)
		{
			USceneComponent* AttachSceneComponent = GetParentComponent()->GetAttachParent();
			GetRootComponent()->AttachToComponent(AttachSceneComponent, FAttachmentTransformRules::KeepWorldTransform);
		}
	}

	if (StaticMeshComponents.Num() <= 1)
		return;
	
	if (AttachEnd)
	{
		UStaticMeshComponent* EndStaticMeshComponent = StaticMeshComponents[StaticMeshComponents.Num() - 1];
		EndStaticMeshComponent->SetSimulatePhysics(false);
		AActor* OwingActor = GetParentActor();

		if (OwingActor == nullptr)
			return;
		
		TArray<UActorComponent*> Components;
		OwingActor->GetComponents(Components);

		for (UActorComponent* Component : Components)
		{
			GEngine->AddOnScreenDebugMessage(-1,5,FColor::Yellow,Component->GetName());

			if (Component->GetName() == AttachEndComponentName)
			{
				if (USceneComponent* SceneComponent = Cast<USceneComponent>(Component))
				{
					EndStaticMeshComponent->AttachToComponent(SceneComponent,
					FAttachmentTransformRules::KeepRelativeTransform);
					GEngine->AddOnScreenDebugMessage(-1,5,FColor::Blue,SceneComponent->GetName());

					if (UPrimitiveComponent* PrimitiveComponent = Cast<UPrimitiveComponent>(Component))
					{
						GEngine->AddOnScreenDebugMessage(-1,5,FColor::Blue,PrimitiveComponent->GetName());
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

void AChain::CreatePhysicsConstraint(UPrimitiveComponent* FirstComponent, UPrimitiveComponent* SecondComponent,
                                     FVector Location, bool IsRelativeLocation)
{
	UPhysicsConstraintComponent* PhysicsConstraint = NewObject<UPhysicsConstraintComponent>(this);
	PhysicsConstraint->RegisterComponent();
	PhysicsConstraint->AttachToComponent(CustomRootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	PhysicsConstraint->SetConstrainedComponents(FirstComponent, NAME_None, SecondComponent, NAME_None);
	PhysicsConstraints.Add(PhysicsConstraint);
	IsRelativeLocation?PhysicsConstraint->SetRelativeLocation(Location):PhysicsConstraint->SetWorldLocation(Location);
	PhysicsConstraint->SetDisableCollision(true);

	PhysicsConstraint->SetAngularTwistLimit(ConstraintTwistMotion, TwistLimitAngle);
	PhysicsConstraint->SetAngularSwing1Limit(ConstraintSwing1Motion, Swing1LimitAngle);
	PhysicsConstraint->SetAngularSwing2Limit(ConstraintSwing2Motion, Swing2LimitAngle);
}
