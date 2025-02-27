#include "../../Spells/ProjectileSpell/BaseProjectileSpell.h"

#include "AbilitySystemComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
 
ABaseProjectileSpell::ABaseProjectileSpell()
{
	SphereCollider = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere Collider"));
	SetRootComponent(SphereCollider);
	SphereCollider->OnComponentBeginOverlap.AddDynamic(this,&ABaseProjectileSpell::OnSphereBeginOverlap);
	SphereCollider->SetCollisionProfileName(FName("Spells"));
	SphereCollider->CanCharacterStepUpOn = ECB_No;
	
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(RootComponent);
	Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile Component"));
	ProjectileMovement->UpdatedComponent = RootComponent;
	ProjectileMovement->InitialSpeed = 3000;
	ProjectileMovement->MaxSpeed = 3000;
	ProjectileMovement->bRotationFollowsVelocity = true;
}

void ABaseProjectileSpell::OnOverlapActor(AActor* CollideActor)
{
	
}

void ABaseProjectileSpell::OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
                                                UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	OnOverlapActor(OtherActor);
}
