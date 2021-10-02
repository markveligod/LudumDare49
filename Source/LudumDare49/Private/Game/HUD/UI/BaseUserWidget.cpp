// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/HUD/UI/BaseUserWidget.h"
#include "Game/GameJamModeBase.h"

void UBaseUserWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();
    this->GameMode = Cast<AGameJamModeBase>(GetWorld()->GetAuthGameMode());
    checkf(this->GameMode, TEXT("Game mode is nullptr"));
}


