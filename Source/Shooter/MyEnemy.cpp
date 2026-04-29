#include "MyEnemy.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h" 
#include "Kismet/GameplayStatics.h"
#include "MyGameInstance.h"
#include "MyCharacter.h"
#include "MyGameMode.h"
#include "DamageFlashWidget.h"
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

	GM = Cast<AMyGameMode>(UGameplayStatics::GetGameMode(GetWorld()));

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

	UE_LOG(LogTemp, Warning, TEXT("%s shot at the player"), *GetName());

	UGameplayStatics::ApplyDamage(
		Player,
		IncomingDamage,
		GetController(),
		this,
		nullptr
	);
}

float AMyEnemy::TakeDamage(
	float DamageAmount,
	FDamageEvent const& DamageEvent,
	AController* EventInstigator,
	AActor* DamageCauser)
{
	CurrentHealth -= DamageAmount;

	if (CurrentHealth <= 0.f)
	{
		Die();
	}

	return DamageAmount;
}

void AMyEnemy::Die()
{
	UE_LOG(LogTemp, Warning, TEXT("%s has died!"), *GetName());

	if ( !GM ) GM = Cast<AMyGameMode>(UGameplayStatics::GetGameMode(GetWorld()));

	if (GM)
	{
		GM->EnemiesLeft--;
		UE_LOG(LogTemp, Warning, TEXT("Enemies left: %d"), GM->EnemiesLeft);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("GM was null, skipping"));
	}

	SetActorHiddenInGame(true);
	SetActorEnableCollision(false);
	GetWorldTimerManager().ClearTimer(ShootTimerHandle);

	if (GM->EnemiesLeft <= 0) GM->ResetAll();
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