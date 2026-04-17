// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputAction.h"
#include "InputMappingContext.h"
#include "InputActionValue.h"
#include "MyPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTER_API AMyPlayerController : public APlayerController
{
	GENERATED_BODY()
	

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void SetupInputComponent() override;
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void Shoot();
	void Reload();
	void FinishReload();

private:
	UPROPERTY(EditAnywhere)
	UInputMappingContext* MappingContext;

	UPROPERTY(EditAnywhere)
	UInputAction* MoveAction;

	UPROPERTY(EditAnywhere)
	UInputAction* LookAction;

	UPROPERTY(EditAnywhere)
	UInputAction* ShootAction;

	UPROPERTY(EditAnywhere)
	UInputAction* ReloadAction;

	FTimerHandle ReloadTimerHandle;
	bool IsReloading = false;
	


};
