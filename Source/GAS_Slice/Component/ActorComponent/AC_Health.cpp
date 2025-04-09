#include "../../Component/ActorComponent/AC_Health.h"

UAC_Health::UAC_Health()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UAC_Health::TakeDamage()
{
	Death();
}

void UAC_Health::Death() const
{
	OnDeath.Broadcast();
}

