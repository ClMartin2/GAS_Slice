#include "../../Character/Enemy/Enemy.h"
#include "Components/SkeletalMeshComponent.h"
#include "../../Component/StaticMeshComponent/SMC_Gun.h"

AEnemy::AEnemy()
{
	PrimaryActorTick.bCanEverTick = false;

	BodyVisual = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Body_Visual"));
	BodyVisual->SetupAttachment(RootComponent);

	SMCGun = CreateDefaultSubobject<USMC_Gun>(TEXT("Gun"));
	SMCGun->SetupAttachment(BodyVisual);

	BarrelPosition = CreateDefaultSubobject<USceneComponent>(TEXT("Barrel Position"));
	BarrelPosition->SetupAttachment(SMCGun);

	SMCGun->SetBarrelPostion(BarrelPosition);
}

void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	SMCGun->SetAbilitySystemComponent(GetAbilitySystemComponent());
}

void AEnemy::Death_Implementation()
{
	Super::Death_Implementation();
	Destroy();
}
