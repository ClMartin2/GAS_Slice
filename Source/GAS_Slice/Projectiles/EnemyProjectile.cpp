// Fill out your copyright notice in the Description page of Project Settings.


#include "../Projectiles/EnemyProjectile.h"

#include "AttributeSet.h"
#include "../Library/GAS_Utils.h"
#include "Components/SphereComponent.h"
#include "GAS_Slice/GAS/AttributeSet/BasicAttributeSet.h"

void AEnemyProjectile::BeginPlay()
{
	Super::BeginPlay();
	SetActorEnableCollision(false);
	CollisionComp->OnComponentBeginOverlap.AddDynamic(this, &AEnemyProjectile::OnBoxBeginOverlap);
}

void AEnemyProjectile::OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UAbilitySystemComponent* TargetASC = GAS_Utils::GetAbilitySystem(OtherActor);
    
	if (TargetASC)
	{
		float Shield = 0.0f;
		float Health = 0.0f;

		FGameplayAttribute ShieldAttr = UBasicAttributeSet::GetShieldAttribute();
		Shield = TargetASC->GetNumericAttribute(ShieldAttr);
	
		float ShieldDamage = FMath::Min(Shield, Damage);
		float RemainingDamage = Damage - ShieldDamage;
	
		if (ShieldDamage > 0)
		{
			GAS_Utils::ApplyGameplayEffectToTargetSetByCaller(this,OtherActor,
			GameplayEffectClassShield,EnemyAbilitySystemComponent,-ShieldDamage,FName("Attribute.Shield"));
		}
	
		if (RemainingDamage > 0)
		{
			GAS_Utils::ApplyGameplayEffectToTargetSetByCaller(this,OtherActor,
			GameplayEffectClassHealth,EnemyAbilitySystemComponent,-RemainingDamage,FName("Attribute.Health"));
		}
	}
	
	Death();
}


void AEnemyProjectile::Death()
{
	Destroy();
}
