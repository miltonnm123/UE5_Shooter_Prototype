#include "MyEnemy.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h" 
#include "Kismet/GameplayStatics.h"
#include "MyGameInstance.h"
#include "MyCharacter.h"
#include "MyGameMode.h"
#include "GameFramework/Character.h"


AMyEnemy::AMyEnemy()
{
	PrimaryActorTick.bCanEverTick = false;

	GetCharacterMovement()->DisableMovement();

	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
}

void AMyEnemy::BeginPlay()
{
	Super::BeginPlay();
	CurrentHealth = MaxHealth;
	StartLocation = GetActorLocation();

	UMyGameInstance* GI = Cast<UMyGameInstance>(GetGameInstance());
	if (GI) ShootInterval = GI->SelectedShootInterval;

	UE_LOG(LogTemp, Warning, TEXT("Selected Shoot Interval: %f"), GI->SelectedShootInterval);

	GetWorldTimerManager().SetTimer(
		ShootTimerHandle,
		this,
		&AMyEnemy::ShootAtPlayer,
		ShootInterval,
		true
	);
}

bool AMyEnemy::HasLineOfSightToPlayer()
{
	ACharacter* Player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);

	if (!Player) return false;

	FHitResult HitResult;
	FVector Start = GetActorLocation();
	FVector End = Player->GetActorLocation();

	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);

	GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility, Params);

	return !HitResult.bBlockingHit || HitResult.GetActor() == Player;
}

void AMyEnemy::ShootAtPlayer()
{
	if (!HasLineOfSightToPlayer()) return;

	ACharacter* Player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	
	if (!Player) return;

	float Distance = FVector::Dist(GetActorLocation(), Player->GetActorLocation());
	if (Distance > ShootRange) return;

	AMyCharacter* MyPlayer = Cast<AMyCharacter>(Player);
	if (MyPlayer)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s shot at the player"), *GetName());
		MyPlayer->CurrentHealth -= DamageAmount;

		if (MyPlayer->CurrentHealth <= 0)
		{
			UE_LOG(LogTemp, Log, TEXT("player died!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"));
			AMyGameMode* GameMode = Cast<AMyGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
			if (GameMode)
			{
				GameMode->ResetAll();
			}
		}
	}
}

void AMyEnemy::TakeDamageFromPlayer(float dmg)
{
	if (CurrentHealth <= 0.f) return;

	CurrentHealth -= dmg;
	UE_LOG(LogTemp, Warning, TEXT("%s hit! Health: %.1f / %.1f"),
		*GetName(), CurrentHealth, MaxHealth);

	if (CurrentHealth <= 0.f)
	{
		Die();
	}
}

void AMyEnemy::Die()
{
	UE_LOG(LogTemp, Warning, TEXT("%s has died!"), *GetName());
	SetActorHiddenInGame(true);
	SetActorEnableCollision(false);
	GetWorldTimerManager().ClearTimer(ShootTimerHandle);
}

void AMyEnemy::ResetEnemy()
{
	CurrentHealth = MaxHealth;

	SetActorLocation(StartLocation);
	SetActorHiddenInGame(false);
	SetActorEnableCollision(true);

	GetWorldTimerManager().SetTimer(
		ShootTimerHandle,
		this,
		&AMyEnemy::ShootAtPlayer,
		ShootInterval,
		true
	);


}