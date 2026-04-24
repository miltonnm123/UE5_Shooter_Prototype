#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "MyEnemy.h"
#include "MyGameMode.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTER_API AMyGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AMyGameMode();

	void ResetEnemies();
	void ResetPlayer();
	void ResetAll();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<AActor*> Enemies;

protected:

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	
};
