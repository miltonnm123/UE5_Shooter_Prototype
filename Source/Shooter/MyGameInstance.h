// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "MyGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTER_API UMyGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, Category = "Settings")
	int32 SelectedDifficulty = 2;


	UPROPERTY(BlueprintReadWrite, Category = "Settings")
	float SelectedShootInterval = 2.0f;

	UPROPERTY(BlueprintReadWrite, Category = "Data")
	float CurrentRunTime = 0.0f;

	UPROPERTY(BlueprintReadWrite, Category = "Data")
	float BestRunTime = 0.0f;
	
};
