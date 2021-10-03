// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/GoodBall/GoodBallPlayerController.h"
#include "Game/GameJamModeBase.h"

void AGoodBallPlayerController::BeginPlay()
{
    Super::BeginPlay();
    
    this->GameMode = Cast<AGameJamModeBase>(GetWorld()->GetAuthGameMode());
    checkf(this->GameMode, TEXT("Game mode is nullptr"));
    this->GameMode->OnGameLevelStateChanged.AddUObject(this, &AGoodBallPlayerController::OnChangeGameState);
}

void AGoodBallPlayerController::OnChangeGameState(EGameLevelState NewState)
{
    if (NewState == EGameLevelState::InProgress)
    {
        SetInputMode(FInputModeGameOnly());
        bShowMouseCursor = false;
    }
    else
    {
        bShowMouseCursor = true;
        SetInputMode(FInputModeUIOnly());
    }
}



