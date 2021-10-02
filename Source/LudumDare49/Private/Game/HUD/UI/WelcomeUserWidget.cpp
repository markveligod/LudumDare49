// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/HUD/UI/WelcomeUserWidget.h"
#include "Components/Button.h"
#include "Game/GameJamModeBase.h"
#include "Components/Image.h"

void UWelcomeUserWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();
    this->ContinueButton->OnClicked.AddDynamic(this, &UWelcomeUserWidget::ChangeGameState);
}

void UWelcomeUserWidget::ChangeGameState()
{
    GetGameMode()->ChangeGameState(EGameLevelState::InProgress);
}

void UWelcomeUserWidget::ResetTimer()
{
    GetWorld()->GetTimerManager().ClearTimer(this->TimerDelay);
    this->BlackImage->SetVisibility(ESlateVisibility::Hidden);
}

void UWelcomeUserWidget::ShowAnimStart()
{
    Super::ShowAnimStart();
    GetWorld()->GetTimerManager().SetTimer(this->TimerDelay, this, &UWelcomeUserWidget::ResetTimer, MapTimeOutRate[EStateAnimPlay::StartAnim]);
}

void UWelcomeUserWidget::ShowAnimEnd()
{
    this->BlackImage->SetVisibility(ESlateVisibility::Visible);
    Super::ShowAnimEnd();
}
