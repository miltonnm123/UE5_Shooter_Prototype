// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Camera/CameraComponent.h"
#include "Sound/SoundBase.h"
#include "DamageFlashWidget.h"
#include "Kismet/GameplayStatics.h"
#include "MyCharacter.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAmmoChanged, int, NewAmmo);

UCLASS()
class SHOOTER_API AMyCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMyCharacter();

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnAmmoChanged OnAmmoChanged;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	int CurrentAmmo = 30;
	int MaxAmmo = 30;

	float WeaponRange = 10000.0f;
	
	float RisePerShot = 0.04f;
	float Heat = 0.0f;
	float DecayRate = 6.0f;
	float HalfAngle = FMath::DegreesToRadians(0.5f);

	void SetAmmo(int NewAmmo);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
	USoundBase* FireSound;

	UPROPERTY(EditDefaultsOnly, Category = "Animations")
	UAnimMontage* ReloadMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Animations")
	UAnimMontage* ShootMontage;

	UPROPERTY(VisibleAnywhere)
	USkeletalMeshComponent* WeaponMesh;

	float CurrentHealth;
	float MaxHealth = 100;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	float TakeDamage(
		float DamageAmount,
		FDamageEvent const& DamageEvent,
		AController* EventInstigator,
		AActor* DamageCauser);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void PlayerShoot();

	void PlayReloadAnimation();

private:
	UPROPERTY(VisibleAnywhere)
	UCameraComponent* Camera;

	UPROPERTY(EditAnywhere)
	float Range = 7000.0f;

	UPROPERTY(VisibleAnywhere)
	float LastShootTime = 0.0f;

	UPROPERTY(EditAnywhere)
	float FireRate = 0.1f;




};
