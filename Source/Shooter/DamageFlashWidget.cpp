// Fill out your copyright notice in the Description page of Project Settings.


#include "DamageFlashWidget.h"

void UDamageFlashWidget::PlayFlash()
{
    UE_LOG(LogTemp, Warning, TEXT("FlashAnim: %s"),
        FlashAnim ? TEXT("valid") : TEXT("null"));

    if (FlashAnim)
    {
        PlayAnimation(FlashAnim);
        UE_LOG(LogTemp, Warning, TEXT("Playing flash"));
    }
}

void UDamageFlashWidget::NativeConstruct()
{
    Super::NativeConstruct();

    if (FlashImage) FlashImage->SetRenderOpacity(0.0f);
}



