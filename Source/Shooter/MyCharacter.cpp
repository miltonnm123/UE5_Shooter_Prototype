// Fill out your copyright notice in the Description page of Project Settings.


#include "MyCharacter.h"
#include "MyEnemy.h"
#include "MyPlayerController.h"
#include "MyGameMode.h"
#include "MyGameInstance.h"
#include "DrawDebugHelpers.h"


// Sets default values
AMyCharacter::AMyCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));

	Camera->SetupAttachment(GetRootComponent());
	Camera->SetRelativeLocation(FVector(0, 0, 100));

	Camera->bUsePawnControlRotation = true;
	bUseControllerRotationYaw = true;
	bUseControllerRotationPitch = true;

	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh"));
	WeaponMesh->SetupAttachment(Camera);
}

// Called when the game starts or when spawned
void AMyCharacter::BeginPlay()
{
	Super::BeginPlay();

	CurrentHealth = MaxHealth;
	
}

// Called every frame
void AMyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	Heat = FMath::Max(0.0f, Heat - DecayRate * DeltaTime);

}

// Called to bind functionality to input
void AMyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AMyCharacter::PlayerShoot()
{
	if (CurrentAmmo <= 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("No ammo, press R to reload..."));
		return;
	}



	float CurrentTime = GetWorld()->GetTimeSeconds();

	UE_LOG(LogTemp, Warning, TEXT("CurrentTime: %f, LastShootTime: %f, Diff: %f"), CurrentTime, LastShootTime, CurrentTime - LastShootTime);

	if (CurrentTime - LastShootTime >= FireRate)
	{
		UE_LOG(LogTemp, Warning, TEXT("Heat before coming shot: %f"), Heat);

		float HorizontalScatter = FMath::FRandRange(-HalfAngle, HalfAngle) * Heat;

		float VerticalClimb = Heat * RisePerShot;

		FVector Forward = Camera->GetForwardVector();
		FVector Right = Camera->GetRightVector();
		FVector Up = FVector::CrossProduct(Forward, Right);

		FVector SpreadDir = Forward + Right * FMath::Sin(HorizontalScatter) + Up * VerticalClimb;

		SpreadDir.Normalize();


		FVector start = Camera->GetComponentLocation();
		FVector end = start + SpreadDir * WeaponRange;

		FHitResult HitResult;

		FVector Location = Camera->GetComponentLocation();

		float Pitch = FMath::FRandRange(0.95f, 1.05f);
		float Volume = FMath::FRandRange(0.65f, 0.75f);

		UAnimInstance* AnimInstance = WeaponMesh->GetAnimInstance();

		if (FireSound && ShootMontage)
		{
			UGameplayStatics::PlaySoundAtLocation(this, FireSound, Location, Volume, Pitch);
			float Duration = AnimInstance->Montage_Play(ShootMontage, 1.0f);
			UE_LOG(LogTemp, Warning, TEXT("Montage_Play returned: %f"), Duration);
		}

		GetWorld()->LineTraceSingleByChannel(HitResult, start, end, ECC_Visibility);

		DrawDebugLine(GetWorld(), start, end, FColor::Red, false, 1.0f);

		if (HitResult.bBlockingHit)
		{
			//UE_LOG(LogTemp, Warning, TEXT("Hit: %s"), *HitResult.GetActor()->GetName());

			AMyEnemy* Enemy = Cast<AMyEnemy>(HitResult.GetActor());

			if (Enemy)
			{
				UGameplayStatics::ApplyDamage(
					Enemy,
					50.0f,
					GetController(),
					this,
					nullptr
				);
			}
		}

		LastShootTime = CurrentTime;
		CurrentAmmo--;
		Heat++;
	}

	
}

void AMyCharacter::SetAmmo(int NewAmmo)
{
	CurrentAmmo = NewAmmo;

	OnAmmoChanged.Broadcast(CurrentAmmo);
}

void AMyCharacter::PlayReloadAnimation()
{
	UAnimInstance* AnimInstance = WeaponMesh->GetAnimInstance();

	UE_LOG(LogTemp, Warning, TEXT("AnimInstance: %s, ReloadMontage: %s"),
		AnimInstance ? TEXT("valid") : TEXT("null"),
		ReloadMontage ? TEXT("valid") : TEXT("null"));

	if (AnimInstance && ReloadMontage)
	{
		float Duration = AnimInstance->Montage_Play(ReloadMontage, 1.0f);
		UE_LOG(LogTemp, Warning, TEXT("Montage_Play returned: %f"), Duration);
	}
}

float AMyCharacter::TakeDamage(
	float DamageAmount,
	FDamageEvent const& DamageEvent,
	AController* EventInstigator,
	AActor* DamageCauser)
{
	CurrentHealth -= DamageAmount;

	AMyPlayerController* PC = Cast<AMyPlayerController>(GetController());
	if (PC)
	{
		PC->ShowDamageFlash();
	}

	if (CurrentHealth <= 0)
	{
		AMyGameMode* GM = Cast<AMyGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
		UMyGameInstance* GI = Cast<UMyGameInstance>(GetGameInstance());
		GM->GameIsRunning = false;
		GI->CurrentRunTime = 0;
		GM->GoToMainMenuLevel();
		//if (GM) GM->ResetAll();
	}

	return DamageAmount;
}

