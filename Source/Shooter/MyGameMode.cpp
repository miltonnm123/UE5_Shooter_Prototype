#include "MyGameMode.h"
#include "MyCharacter.h"
#include "MyGameInstance.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/PlayerStart.h"

AMyGameMode::AMyGameMode()
{
	PrimaryActorTick.bCanEverTick = true; // GameMode tick is OFF by default
}

void AMyGameMode::BeginPlay()
{
	Super::BeginPlay();

	UMyGameInstance* GI = Cast<UMyGameInstance>(GetGameInstance());
	if (GI) CurrentDifficulty = GI->SelectedDifficulty;

	UE_LOG(LogTemp, Warning, TEXT("Selected Difficulty: %d"), GI->SelectedDifficulty);

	SpawnAllEnemies(CurrentDifficulty);
	UE_LOG(LogTemp, Warning, TEXT("ENEMIES SUCCESSFULLY SPAWNED"));

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
	GoToMainMenuLevel();
}

void AMyGameMode::SpawnAllEnemies(int32 Difficulty)
{
	TArray<AActor*> SpawnPoints;
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName("SpawnPoint"), SpawnPoints);

	UE_LOG(LogTemp, Warning, TEXT("SpawnPoints found: %d"), SpawnPoints.Num());
	UE_LOG(LogTemp, Warning, TEXT("EnemyClass set: %s"), EnemyClass ? TEXT("YES") : TEXT("NO"));
	UE_LOG(LogTemp, Warning, TEXT("SpawnCount: %d"), SpawnCount);

	if (!EnemyClass)
	{
		UE_LOG(LogTemp, Warning, TEXT("EnemySpawner: No EnemyClass set"));
		return;
	}

	if (Difficulty == HardDifficulty)
	{
		SpawnCount *= 2;
	}

	if (Difficulty == MediumDifficulty)
	{
		SpawnCount *= 1.5f;
	}

	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	TArray<AActor*> AvailablePoints = SpawnPoints;
	for (int32 i = AvailablePoints.Num() - 1; i > 0; i--)
	{
		int32 j = FMath::RandRange(0, i);
		AvailablePoints.Swap(i, j);
	}

	int32 ActualSpawnCount = FMath::Min(SpawnCount, AvailablePoints.Num());

	EnemiesLeft = ActualSpawnCount;

	for (int32 i = 0; i < ActualSpawnCount; i++)
	{
		AActor* SpawnPoint = AvailablePoints[i];

		if (!IsValid(SpawnPoint)) continue;

		FVector Location = SpawnPoint->GetActorLocation();
		FRotator Rotation = SpawnPoint->GetActorRotation();

		AActor* SpawnedEnemy = GetWorld()->SpawnActor<AActor>(EnemyClass, Location, Rotation, SpawnParams);

		if (SpawnedEnemy)
		{
			UE_LOG(LogTemp, Log, TEXT("Spawned enemy at %s"), *Location.ToString());
		}
	}
}

void AMyGameMode::SetCurrentDifficulty(int NewValue)
{
	CurrentDifficulty = NewValue;
}

void AMyGameMode::GoToMainMenuLevel()
{
	UGameplayStatics::OpenLevel(GetWorld(), FName("MainMenuLevel"));
}

