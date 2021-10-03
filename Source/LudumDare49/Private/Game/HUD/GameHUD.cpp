// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/HUD/GameHUD.h"
#include "Game/HUD/UI/BaseUserWidget.h"
#include "Game/GameJamModeBase.h"


DEFINE_LOG_CATEGORY_STATIC(LogGameHUD, All, All);

void AGameHUD::BeginPlay()
{
    Super::BeginPlay();
    check(this->GameProgressWidgetClass);
    check(this->WelcomeWidgetClass);
    check(this->GameOverWidgetClass);

    this->GameWidgets.Add(EGameLevelState::InProgress, CreateWidget<UBaseUserWidget>(GetWorld(), this->GameProgressWidgetClass));
    this->GameWidgets.Add(EGameLevelState::WaitToStart, CreateWidget<UBaseUserWidget>(GetWorld(), this->WelcomeWidgetClass));
    this->GameWidgets.Add(EGameLevelState::GameOver, CreateWidget<UBaseUserWidget>(GetWorld(), this->GameOverWidgetClass));

    // Set Hidden and add to viewport
    for (auto Widget : this->GameWidgets)
    {
        const auto ValueWidget = Widget.Value;
        ValueWidget->AddToViewport();
        ValueWidget->SetVisibility(ESlateVisibility::Hidden);
    }

    this->GameMode = Cast<AGameJamModeBase>(GetWorld()->GetAuthGameMode());
    checkf(this->GameMode, TEXT("Game Mode is nullptr"));
    this->GameMode->OnGameLevelStateChanged.AddUObject(this, &AGameHUD::OnChangedGameState);
}

void AGameHUD::OnChangedGameState(EGameLevelState NewState)
{
    this->GameState = NewState;
    UE_LOG(LogGameHUD, Display, TEXT("New Game State: %s"), *UEnum::GetValueAsString(NewState));
    if (this->WidgetOnVisible)
    {
        this->WidgetOnVisible->ShowAnimEnd();
        GetWorld()->GetTimerManager().SetTimer(this->TimerHandleSwitch, this, &AGameHUD::ResetToEndAnimation,
            this->WidgetOnVisible->MapTimeOutRate[EStateAnimPlay::EndAnim]);
    }
    else
    {
        this->SetupWidgetMenu();
    }
}

void AGameHUD::ResetToEndAnimation()
{
    GetWorld()->GetTimerManager().ClearTimer(this->TimerHandleSwitch);
    this->SetupWidgetMenu();
}

void AGameHUD::ResetToStartAnim()
{
    GetWorld()->GetTimerManager().ClearTimer(this->TimerHandleSwitch);
}

void AGameHUD::SetupWidgetMenu()
{
    if (this->WidgetOnVisible) this->WidgetOnVisible->SetVisibility(ESlateVisibility::Hidden);

    this->WidgetOnVisible = this->GameWidgets[this->GameState];
    this->WidgetOnVisible->SetVisibility(ESlateVisibility::Visible);
    this->WidgetOnVisible->ShowAnimStart();
   GetWorld()->GetTimerManager().SetTimer(
        this->TimerHandleSwitch, this, &AGameHUD::ResetToStartAnim, this->WidgetOnVisible->MapTimeOutRate[EStateAnimPlay::StartAnim]);
}
