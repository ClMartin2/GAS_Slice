#include "../CustomAIController/AICEnemy.h"

AAICEnemy::AAICEnemy()
{
	SetGenericTeamId(FGenericTeamId(TeamId));
}

void AAICEnemy::BeginPlay()
{
	Super::BeginPlay();
}


ETeamAttitude::Type AAICEnemy::GetTeamAttitudeTowards(const AActor& Other) const
{
	if (const APawn* OtherPawn = Cast<APawn>(&Other))
	{
		if (const IGenericTeamAgentInterface* TeamAgent = Cast<IGenericTeamAgentInterface>(OtherPawn->GetController()))
		{
			FGenericTeamId OtherTeamID = TeamAgent->GetGenericTeamId();
			
			if (OtherTeamID == FGenericTeamId(TeamId))
				return ETeamAttitude::Friendly;
			else
				return ETeamAttitude::Hostile;
		}
	}

	return ETeamAttitude::Neutral;
}

