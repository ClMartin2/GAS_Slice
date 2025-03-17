#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GAS_Slice/Chain.h"
#include "Knife.generated.h"

class AChain;
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

	UPROPERTY(EditAnywhere, Category = "Settings")
	FVector OffsetBoxExtentCollisionAttack = FVector::Zero();

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly, Category = Collision, meta = (AllowPrivateAccess = "true"))
	UBoxComponent* BoxCollision;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly, Category = "Visual", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* StaticMeshKnife_;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	UProjectileMovementComponent* ProjectileMovement;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Settings", meta = (AllowPrivateAccess = "true"))
	UChildActorComponent* Chain;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AChain> ChainClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Actor", meta = (AllowPrivateAccess = "true"))
	AChain* BP_Chain;

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

	UFUNCTION(BlueprintCallable, Category = "Knife")
	void ResetKnife();

	bool GetIsAttached() const {return IsAttached;}

	void CheckCollisionAttack();
	void FinishCheckCollisionAttack();
	void SetAbilitySystemComponent(UAbilitySystemComponent* AbilitySystemComponent){PlayerAbilitySystemComponent = AbilitySystemComponent;}

	UFUNCTION(BlueprintCallable,Category="Getter",meta = (allowPrivateAccess = "true"))
	AChain* GetBPChain() const {return BP_Chain;}
	
protected:
	virtual void BeginPlay() override;
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Knife")
	void StopMove();

	virtual void StopMove_Implementation();
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Knife")
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	void OnHit_Implementation(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
	
private:
	void HitRotate(const FHitResult& Hit);
	void ReplaceHitKnife(const FHitResult& Hit);
	void MakeDamage(FHitResult OutHit);
};
