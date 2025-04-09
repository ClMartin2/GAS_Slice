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

public:
	void TakeDamage();
	void Death() const;
};
