// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/HUD/UI/GameOverUserWidget.h"

#include "Components/Button.h"
#include "Components/Image.h"
#include "Kismet/GameplayStatics.h"
#include "Menu/MSBJGameInstance.h"

void UGameOverUserWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();
    this->MainMenuButton->OnClicked.AddDynamic(this, &UGameOverUserWidget::ClickButtonToMenu);
    this->RestartButton->OnClicked.AddDynamic(this, &UGameOverUserWidget::RestartGame);
}

void UGameOverUserWidget::ShowAnimStart()
{
    GetWorld()->GetTimerManager().SetTimer(this->TimerOnStart, this, &UGameOverUserWidget::HideBlackImage, MapTimeOutRate[EStateAnimPlay::StartAnim]);
    Super::ShowAnimStart();
}

void UGameOverUserWidget::ClickButtonToMenu()
{
    UGameplayStatics::OpenLevel(GetWorld(), GetGameInstance()->NameMenuLevel);
}

void UGameOverUserWidget::RestartGame()
{
    UGameplayStatics::OpenLevel(GetWorld(), GetGameInstance()->NameStartLevel);
}

void UGameOverUserWidget::HideBlackImage()
{
    GetWorld()->GetTimerManager().ClearTimer(this->TimerOnStart);
    this->BlackImage->SetVisibility(ESlateVisibility::Hidden);
}
