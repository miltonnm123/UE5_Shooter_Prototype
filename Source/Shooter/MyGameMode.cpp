#include "MyGameMode.h"
#include "MyCharacter.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/PlayerStart.h"

AMyGameMode::AMyGameMode()
{
	PrimaryActorTick.bCanEverTick = true; // GameMode tick is OFF by default
}

void AMyGameMode::BeginPlay()
{
	Super::BeginPlay();


	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AMyEnemy::StaticClass(), Enemies);
}

void AMyGameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector MyCharacterPosition = GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation();

	for (AActor* Actor : Enemies)
	{
		AMyEnemy* Enemy = Cast<AMyEnemy>(Actor);
		if (Enemy)
		{
			FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(Enemy->GetActorLocation(), MyCharacterPosition);
			Enemy->SetActorRotation(LookAtRotation);
		}
	}
}

void AMyGameMode::ResetEnemies()
{

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

