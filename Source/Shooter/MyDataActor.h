// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Kismet/GameplayStatics.h"
#include "MyDataActor.generated.h"

UCLASS()
class SHOOTER_API AMyDataActor : public AActor
{
	GENERATED_BODY()

	
public:	
	AMyDataActor();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	TArray<AActor*> StoredSpawnPoints;

protected:
	virtual void BeginPlay() override;

};
