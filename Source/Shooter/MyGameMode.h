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
	UFUNCTION()

	void ResetEnemies();
	void ResetPlayer();
	void ResetAll();
	
};
