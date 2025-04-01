#include "EnemySpawner.h"
#include "Character/Enemy/Enemy.h"
#include "DrawDebugHelpers.h"

AEnemySpawner::AEnemySpawner()
{
	PrimaryActorTick.bCanEverTick = true; // Active Tick même en mode éditeur
}

#if WITH_EDITOR
void AEnemySpawner::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	if (GetWorld() && GetWorld()->WorldType == EWorldType::Editor)
	{
		DrawDebugCircle(
			GetWorld(),
			GetActorLocation(),
			RadiusEnemyToSpawn,
			50,
			FColor::Red,
			false,
			10,
			0,
			2.f,
			FVector(1, 0, 0),
			FVector(0, 1, 0),
			true
		);
	}
}
#endif

void AEnemySpawner::BeginPlay()
{
	Super::BeginPlay();

	for (int i = 0; i < NumberEnemyToSpawnAtStart; i++) 
	{
		SpawnEnemy();
	}
}

void AEnemySpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	CounterToSpawnEnemy += DeltaTime;

	if (CounterToSpawnEnemy >= SpawnEnemyToXSeconds)
	{
		for (int i = 0; i < NumberEnemyToSpawn; i++)
		{
			SpawnEnemy();
		}

		CounterToSpawnEnemy = 0;
	}
}

void AEnemySpawner::SpawnEnemy()
{
	float RandomAngle = FMath::FRandRange(0.f,360.f);
	float RandomAngleRad = FMath::DegreesToRadians(RandomAngle);
	
	FVector SpawnLocation = GetActorLocation() + FVector(cos(RandomAngleRad) * RadiusEnemyToSpawn,sin(RandomAngleRad) * RadiusEnemyToSpawn,0);
	FRotator SpawnRotation = (GetActorLocation() - SpawnLocation).Rotation();
	
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	GetWorld()->SpawnActor<AEnemy>(EnemyClass, SpawnLocation, SpawnRotation, SpawnParams);
}


