// Fill out your copyright notice in the Description page of Project Settings.


#include "MyDataActor.h"



AMyDataActor::AMyDataActor()
{

}

void AMyDataActor::BeginPlay()
{
	Super::BeginPlay();
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName("SpawnPoint"), StoredSpawnPoints);
}

