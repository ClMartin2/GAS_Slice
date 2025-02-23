#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Knife.generated.h"

class UAbilitySystemComponent;
class UGameplayEffect;
class UProjectileMovementComponent;
class UBoxComponent;
class UStaticMeshComponent;
class UCableComponent;

UCLASS()
class GAS_SLICE_API AKnife : public AActor
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, Category = "Settings|Speed")
	float Speed;

	UPROPERTY(EditAnywhere, Category = "Settings|Speed")
	float MaxSpeed;

	UPROPERTY(EditAnywhere, Category = "Settings")
	float DriveAttach = 30;

	UPROPERTY(EditAnywhere, Category = "Settings")
	bool DebugCollisionAttack = false;

	UPROPERTY(EditAnywhere, Category = "Settings")
	float Damage = 10;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly, Category = Collision, meta = (AllowPrivateAccess = "true"))
	UBoxComponent* BoxCollision;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly, Category = "Visual", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* StaticMeshKnife_;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	UProjectileMovementComponent* ProjectileMovement;

	UPROPERTY(EditAnywhere, Category = "Settings|GAS", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UGameplayEffect> GameplayEffectClass;
	
	FVector CameraForward;
	bool IsAttached;
	bool hasAlreadyAttack = false;

private:
	UAbilitySystemComponent* PlayerAbilitySystemComponent;

public:
	AKnife();	
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Knife")
	void Throw(FVector DirectionThrowKnife, FVector NewCameraForward);

	virtual void Throw_Implementation(FVector DirectionThrowKnife, FVector NewCameraForward);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Knife")
	void ResetKnife();
	virtual void ResetKnife_Implementation();

	bool GetIsAttached() const {return IsAttached;}

	void CheckCollisionAttack();
	void FinishCheckCollisionAttack();
	void SetAbilitySystemComponent(UAbilitySystemComponent* AbilitySystemComponent){PlayerAbilitySystemComponent = AbilitySystemComponent;}
	
protected:

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Knife")
	void StopMove();

	virtual void StopMove_Implementation();

private:
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	void HitRotate(const FHitResult& Hit);
	void ReplaceHitKnife(const FHitResult& Hit);
	void MakeDamage(FHitResult OutHit);
};
