// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "GameplayEffectSpell.h"
#include "BaseProjectileSpell.generated.h"

class USphereComponent;
class UProjectileMovementComponent;

UCLASS()
class GAS_SLICE_API ABaseProjectileSpell : public AGameplayEffectSpell
{
	GENERATED_BODY()

public:
	ABaseProjectileSpell();

private:
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category= "Visual", meta=(AllowPrivateAccess=true))
	UStaticMeshComponent* Mesh;

	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category= "Collider", meta=(AllowPrivateAccess=true))
	USphereComponent* SphereCollider;

	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category= "Projectile", meta=(AllowPrivateAccess=true))
	class UProjectileMovementComponent* ProjectileMovement;

protected:
	virtual void OnOverlapActor(AActor* CollideActor);
	
private :
	UFUNCTION()
	void OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
};
