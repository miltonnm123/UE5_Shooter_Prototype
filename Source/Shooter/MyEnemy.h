#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MyEnemy.generated.h"

UCLASS()
class SHOOTER_API AMyEnemy : public ACharacter
{
	GENERATED_BODY()

public:
	AMyEnemy();

	// Max health of the enemy
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy|Stats")
	float MaxHealth = 100.f;

	// Current health — set to MaxHealth on BeginPlay
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Enemy|Stats")
	float CurrentHealth;

	UPROPERTY(EditAnywhere, Category = "Combat")
	float ShootInterval = 2.0f;

	UPROPERTY(EditAnywhere, Category = "Combat")
	float ShootRange = 5000.0f;

	UPROPERTY(EditAnywhere, Category = "Combat")
	float IncomingDamage = 10.0f;

	UPROPERTY(EditAnywhere, Category = "Combat")
	FVector StartLocation;

	FTimerHandle ShootTimerHandle;

	// Called by the player's shoot logic when this enemy is hit
	float TakeDamage(
		float DamageAmount,
		FDamageEvent const& DamageEvent,
		AController* EventInstigator,
		AActor* DamageCauser);

	bool HasLineOfSightToPlayer();
	void ShootAtPlayer();
	void ResetEnemy();

protected:
	virtual void BeginPlay() override;

private:
	void Die();
};