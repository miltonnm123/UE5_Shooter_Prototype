#include "MyGameMode.h"
#include "MyCharacter.h"
#include "GameFramework/PlayerStart.h"


void AMyGameMode::ResetEnemies()
{
	TArray<AActor*> Enemies;

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AMyEnemy::StaticClass(), Enemies);

	for (AActor* Actor : Enemies)
	{
		AMyEnemy* Enemy = Cast<AMyEnemy>(Actor);
		if (Enemy)
		{
			Enemy->ResetEnemy();
		}
	}
}

void AMyGameMode::ResetPlayer()
{
	AMyCharacter* Player = Cast<AMyCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

	if (Player)
	{
		Player->CurrentHealth = Player->MaxHealth;
		Player->CurrentAmmo = Player->MaxAmmo;

		APlayerStart* PlayerStart = Cast<APlayerStart>(UGameplayStatics::GetActorOfClass(GetWorld(), APlayerStart::StaticClass()));
		if (PlayerStart)
		{
			Player->SetActorLocation(PlayerStart->GetActorLocation());
			Player->SetActorRotation(PlayerStart->GetActorRotation());
		}
	}

}

void AMyGameMode::ResetAll()
{
	ResetPlayer();
	ResetEnemies();
}

