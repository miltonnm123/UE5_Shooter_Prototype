#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "GameFramework/Actor.h"
#include "Kismet/GameplayStatics.h"
#include "MyEnemy.h"
#include "MyGameMode.generated.h"

class UMyGameInstance;

UCLASS()
class SHOOTER_API AMyGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AMyGameMode();

	void ResetEnemies();
	void ResetPlayer();
	void ResetAll();
	void GoToMainMenuLevel();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning")
	int32 SpawnCount = 3;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning")
	int32 EnemiesLeft = SpawnCount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning")
	int32 CurrentDifficulty = 2;

	int32 EasyDifficulty = 1;
	int32 MediumDifficulty = 2;
	int32 HardDifficulty = 3;

	bool GameIsRunning = true;

	UFUNCTION(BlueprintCallable, Category = "Spawning")
	void SetCurrentDifficulty(int NewValue);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<AActor*> Enemies;

protected:

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning")
	TSubclassOf<AActor> EnemyClass;

	UMyGameInstance* GI;


	UFUNCTION(BlueprintCallable, Category = "Spawning")
	void SpawnAllEnemies(int32 Difficulty);

	
};
