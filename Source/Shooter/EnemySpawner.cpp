// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemySpawner.h"
#include "MyGameInstance.h"

// Sets default values
AEnemySpawner::AEnemySpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void AEnemySpawner::BeginPlay()
{
	Super::BeginPlay();

	UMyGameInstance* GI = Cast<UMyGameInstance>(GetGameInstance());
	if (GI) CurrentDifficulty = GI->SelectedDifficulty;

	UE_LOG(LogTemp, Warning, TEXT("Selected Difficulty: %d"), GI->SelectedDifficulty);

	SpawnAllEnemies(CurrentDifficulty);
	
}

void AEnemySpawner::SpawnAllEnemies(int32 Difficulty)
{
	if (!EnemyClass)
	{
		UE_LOG(LogTemp, Warning, TEXT("EnemySpawner: No EnemyClass set"));
		return;
	}

	if (Difficulty == HardDifficulty)
	{
		SpawnCount *= 2;
	}

	if (Difficulty == MediumDifficulty)
	{
		SpawnCount *= 1.5f;
	}

	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	TArray<AActor*> AvailablePoints = SpawnPoints;
	for (int32 i = AvailablePoints.Num() - 1; i > 0; i--)
	{
		int32 j = FMath::RandRange(0, i);
		AvailablePoints.Swap(i, j);
	}

	int32 ActualSpawnCount = FMath::Min(SpawnCount, AvailablePoints.Num());

	for (int32 i = 0; i < ActualSpawnCount; i++)
	{
		AActor* SpawnPoint = AvailablePoints[i];

		if (!IsValid(SpawnPoint)) continue;

		FVector Location = SpawnPoint->GetActorLocation();
		FRotator Rotation = SpawnPoint->GetActorRotation();

		AActor* SpawnedEnemy = GetWorld()->SpawnActor<AActor>(EnemyClass, Location, Rotation, SpawnParams);

		if (SpawnedEnemy)
		{
			UE_LOG(LogTemp, Log, TEXT("Spawned enemy at %s"), *Location.ToString());
		}
	}
}


void AEnemySpawner::SetCurrentDifficulty(int NewValue)
{
	CurrentDifficulty = NewValue;
}

