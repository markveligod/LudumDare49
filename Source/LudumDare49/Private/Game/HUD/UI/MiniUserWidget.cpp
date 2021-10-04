// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/HUD/UI/MiniUserWidget.h"
#include "Game/GameJamModeBase.h"

void UMiniUserWidget::StartTimerGame()
{
    GetWorld()->GetTimerManager().SetTimer(TimerHandleUp, this, &UMiniUserWidget::UpdateTime, this->RateTimeCallUp, true);
}

void UMiniUserWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();
    this->GameMode = Cast<AGameJamModeBase>(GetWorld()->GetAuthGameMode());
    check(this->GameMode);
}

void UMiniUserWidget::UpdateTime()
{
    this->CurrentCount += 1;
    if (this->CurrentCount == this->MaxRunGameOver)
    {
        this->GameMode->ChangeGameState(EGameLevelState::GameOver);
        SetVisibility(ESlateVisibility::Hidden);
    }
    
}
