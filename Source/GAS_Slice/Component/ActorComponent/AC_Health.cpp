#include "../../Component/ActorComponent/AC_Health.h"

UAC_Health::UAC_Health()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UAC_Health::BeginPlay()
{
	Super::BeginPlay();
	CurrentHealth = Health;
}

void UAC_Health::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UAC_Health::TakeDamage(float Damage)
{
	CurrentHealth -= Damage;

	if (CurrentHealth <= 0)
		Death();
}

void UAC_Health::Death() const
{
	OnDeath.Broadcast();
}

