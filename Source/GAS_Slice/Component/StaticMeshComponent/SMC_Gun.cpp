#include "../../Component/StaticMeshComponent/SMC_Gun.h"
#include "GAS_Slice/Projectiles/EnemyProjectile.h"

void USMC_Gun::SetBarrelPostion(USceneComponent* NewBarrelPostion)
{
	BarrelPostion = NewBarrelPostion;
}

void USMC_Gun::Shoot()
{
	FRotator Rotation = GetComponentRotation();
	FVector  Location = BarrelPostion->GetComponentLocation();
	FActorSpawnParameters SpawnInfo;
	AActor* Bullet = GetWorld()->SpawnActor<AActor>(ClassBullet,Location, Rotation, SpawnInfo);
	Cast<AEnemyProjectile>(Bullet)->SetAbilitySystemComponent(EnemyAbilitySystemComponent);
	Bullet->SetActorEnableCollision(true);
}
