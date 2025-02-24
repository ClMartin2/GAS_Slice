#include "../Spells/BaseProjectileSpell.h"

#include "AbilitySystemComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
 
ABaseProjectileSpell::ABaseProjectileSpell()
{
	SphereCollider = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere Collider"));
	SetRootComponent(SphereCollider);

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(RootComponent);
	
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile Component"));
	ProjectileMovement->UpdatedComponent = RootComponent;

	SphereCollider->OnComponentBeginOverlap.AddDynamic(this,&ABaseProjectileSpell::OnSphereBeginOverlap);
}

void ABaseProjectileSpell::OnOverlapActor(AActor* CollideActor)
{
	
}

void ABaseProjectileSpell::OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
                                                UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UAbilitySystemComponent* TargetAbilitySystemComponent = OtherActor->FindComponentByClass<UAbilitySystemComponent>();
	
	if (TargetAbilitySystemComponent != nullptr)
		OnOverlapActor(OtherActor);
}
