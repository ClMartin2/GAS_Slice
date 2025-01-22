#include "../../Component/StaticMeshComponent/SMC_Gun.h"


USMC_Gun::USMC_Gun()
{
}

void USMC_Gun::SetBarrelPostion(USceneComponent* NewBarrelPostion)
{
	BarrelPostion = NewBarrelPostion;
}

void USMC_Gun::Shoot()
{
	FRotator Rotation = GetComponentRotation();
	FVector  Location = BarrelPostion->GetComponentLocation();
	FActorSpawnParameters SpawnInfo;
	GetWorld()->SpawnActor<AActor>(ClassBullet,Location, Rotation, SpawnInfo);
}
