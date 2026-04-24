// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Image.h"
#include "DamageFlashWidget.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTER_API UDamageFlashWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void PlayFlash();

	void NativeConstruct();

	UPROPERTY(Transient, meta = (BindWidgetAnim))
	UWidgetAnimation* FlashAnim;

	UPROPERTY(meta = (BindWidget))
	UImage* FlashImage;

protected:

	
};
