#pragma once

#include "CoreMinimal.h"
#include "Chain.h"
#include "ADynamicChain.generated.h"

UCLASS()
class GAS_SLICE_API ADynamicChain : public AChain
{
	GENERATED_BODY()

private:
	TArray<UStaticMeshComponent*> DynamicStaticMeshComponents;
	TArray<UPhysicsConstraintComponent*> DynamicPhysicsConstraintComponent;
	TMap<UStaticMeshComponent*, UPhysicsConstraintComponent*> DynamicMeshToPhysicsConstraint;
	
public:
	UFUNCTION(BlueprintCallable,Category="Physics",meta=(AllowPrivateAccess=true))
	UStaticMeshComponent* AddDynamicMesh(bool SimulatePhysics, bool _AngularBreakable = false, bool _LinearBreakable = false);
	
	UFUNCTION(BlueprintCallable,Category="Physics",meta=(AllowPrivateAccess=true))
	void DestroyDynamicMesh(UStaticMeshComponent* StaticMeshComponent,bool ClearDynamicArray = true);

	void DestroyAllDynamicMeshes();
};
