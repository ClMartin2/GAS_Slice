#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AC_Health.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDeath);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GAS_SLICE_API UAC_Health : public UActorComponent
{
	GENERATED_BODY()
public:	
	UAC_Health();

public:
	UPROPERTY(EditAnywhere,BlueprintReadWrite,BlueprintAssignable, Category = "Settings", meta = (AllowPrivateAccess = "true"))
	FDeath OnDeath;
	
private:
	UPROPERTY(EditAnywhere,BlueprintReadOnly, Category = "Settings", meta = (AllowPrivateAccess = "true"))
	float Health;

	float CurrentHealth;
	
protected:
	virtual void BeginPlay() override;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	void TakeDamage(float Damage);
	void Death() const;
};
