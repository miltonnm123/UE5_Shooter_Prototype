// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Kismet/GameplayStatics.h"
#include "EnemySpawner.generated.h"

UCLASS()
class SHOOTER_API AEnemySpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEnemySpawner();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning")
	int32 SpawnCount = 3;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning")
	int32 CurrentDifficulty = 2;

	int32 EasyDifficulty = 1;
	int32 MediumDifficulty = 2;
	int32 HardDifficulty = 3;

	UFUNCTION(BlueprintCallable, Category = "Spawning")
	void SetCurrentDifficulty(int NewValue);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning")
	TSubclassOf<AActor> EnemyClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning")
	TArray<AActor*> SpawnPoints;

	UFUNCTION(BlueprintCallable, Category = "Spawning")
	void SpawnAllEnemies(int32 Difficulty);

};
