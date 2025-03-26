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

DECLARE_DELEGATE(FOnChainBreak);

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

	UPROPERTY(EditAnywhere, Category = "Settings|Chain")
	float DelaySetPhysicsChain = 0.3;

	UPROPERTY(EditAnywhere, Category = "Settings|Chain")
	float DelaySetAngularBreakable = 0.1;

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
	bool AngularBreakable = false;
	UStaticMeshComponent* HandStartLocation;
	UAbilitySystemComponent* PlayerAbilitySystemComponent;
	FTimerHandle TimerHandleSetPhysicsHit;
	FTimerHandle TimerHandleSetAngularBreakable;
	

public:
	AKnife();	
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Knife")
	void Throw(FVector DirectionThrowKnife, FVector NewCameraForward);

	virtual void Throw_Implementation(FVector DirectionThrowKnife, FVector NewCameraForward);

	UFUNCTION(BlueprintCallable, Category = "Knife")
	void ResetKnife();

	UFUNCTION(BlueprintCallable, Category = "Knife")
	bool GetIsAttached() const {return IsAttached;}

	void CheckCollisionAttack();
	void FinishCheckCollisionAttack();
	void SetAbilitySystemComponent(UAbilitySystemComponent* AbilitySystemComponent){PlayerAbilitySystemComponent = AbilitySystemComponent;}
	void SetHandStartLocation(UStaticMeshComponent* ComponentHandLocation){HandStartLocation = ComponentHandLocation;}

	UFUNCTION(BlueprintCallable,Category="Getter",meta = (allowPrivateAccess = "true"))
	AChain* GetBPChain() const {return BP_Chain;}

	FOnChainBreak OnChainBreak;
	
protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Knife")
	void StopMove();

	virtual void StopMove_Implementation();
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Knife")
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	void SetChainPhysicsHit();
	void SetAngularBreakable();
	void OnHit_Implementation(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
	
private:
	void HitRotate(const FHitResult& Hit);
	void ReplaceHitKnife(const FHitResult& Hit);
	void MakeDamage(FHitResult OutHit);
	void BreakChain();
};
