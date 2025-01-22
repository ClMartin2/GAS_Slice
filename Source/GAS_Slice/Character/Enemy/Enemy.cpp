#include "../../Character/Enemy/Enemy.h"
#include "Components/StaticMeshComponent.h"
#include "../../Component/StaticMeshComponent/SMC_Gun.h"

AEnemy::AEnemy()
{
	PrimaryActorTick.bCanEverTick = false;
	
	BodyVisual = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Body Visual"));
	BodyVisual->SetupAttachment(RootComponent);
	
	SMCGun = CreateDefaultSubobject<USMC_Gun>(TEXT("Gun"));
	SMCGun->SetupAttachment(BodyVisual);

	BarrelPosition = CreateDefaultSubobject<USceneComponent>(TEXT("Barrel Position"));
	BarrelPosition->SetupAttachment(SMCGun);
	
	SMCGun->SetBarrelPostion(BarrelPosition);
}

