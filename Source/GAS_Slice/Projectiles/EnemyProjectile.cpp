// Fill out your copyright notice in the Description page of Project Settings.


#include "../Projectiles/EnemyProjectile.h"
#include "../Library/Utils.h"
#include "Components/SphereComponent.h"

void AEnemyProjectile::BeginPlay()
{
	Super::BeginPlay();
	SetActorEnableCollision(false);
	CollisionComp->OnComponentBeginOverlap.AddDynamic(this, &AEnemyProjectile::OnBoxBeginOverlap);
}

void AEnemyProjectile::OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Utils::ApplyGameplayEffectToTargetSetByCaller(this,OtherActor,GameplayEffectClass,EnemyAbilitySystemComponent,-Damage,FName("Event.Damage"));
	Death();
}

void AEnemyProjectile::Death()
{
	Destroy();
}
