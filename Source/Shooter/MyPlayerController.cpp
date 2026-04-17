// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPlayerController.h"
#include "GameFramework/Character.h"
#include "MyCharacter.h"


// Called when the game starts or when spawned
void AMyPlayerController::BeginPlay()
{
	Super::BeginPlay();

	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());

	if (Subsystem)
	{
		Subsystem->AddMappingContext(MappingContext, 0);
	}

}

void AMyPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent);

	if (EnhancedInputComponent)
	{
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AMyPlayerController::Move);
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AMyPlayerController::Look);
		EnhancedInputComponent->BindAction(ShootAction, ETriggerEvent::Triggered, this, &AMyPlayerController::Shoot);
		EnhancedInputComponent->BindAction(ReloadAction, ETriggerEvent::Triggered, this, &AMyPlayerController::Reload);
	}
}

void AMyPlayerController::Move(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();

	ACharacter* character = Cast<ACharacter>(GetPawn());

	if (character)
	{
		character->AddMovementInput(character->GetActorForwardVector(), MovementVector.Y);
		character->AddMovementInput(character->GetActorRightVector(), MovementVector.X);
	}
}

void AMyPlayerController::Look(const FInputActionValue& Value)
{
	FVector2D LookVector = Value.Get<FVector2D>();

	AddYawInput(LookVector.X);
	AddPitchInput(LookVector.Y);
}

void AMyPlayerController::Shoot()
{
	if (IsReloading) return;

	AMyCharacter* character = Cast<AMyCharacter>(GetPawn());

	if (character)
	{
		character->PlayerShoot();
	}
}


void AMyPlayerController::Reload()
{
	if (IsReloading) return;

	AMyCharacter* character = Cast<AMyCharacter>(GetPawn());

	if (character)
	{
		IsReloading = true;
		character->PlayReloadAnimation();
		UE_LOG(LogTemp, Warning, TEXT("Reloading..."));

		GetWorldTimerManager().SetTimer(
			ReloadTimerHandle,
			this,
			&AMyPlayerController::FinishReload,
			2.0f,
			false
		);
	}
}

void AMyPlayerController::FinishReload()
{
	AMyCharacter* character = Cast<AMyCharacter>(GetPawn());

	if (character)
	{
		character->CurrentAmmo = character->MaxAmmo;
		IsReloading = false;
		UE_LOG(LogTemp, Warning, TEXT("Reload complete"));
	}
}

