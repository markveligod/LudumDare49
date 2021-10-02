// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/GameJamModeBase.h"
#include "Menu/MSBJGameInstance.h"
#include "GameFramework/GameUserSettings.h"
#include "Kismet/KismetInternationalizationLibrary.h"
#include "Game/HUD/GameHUD.h"
#include "Game/GoodBall/GoodBallPawn.h"
#include "Game/GoodBall/GoodBallPlayerController.h"

AGameJamModeBase::AGameJamModeBase()
{
    HUDClass = AGameHUD::StaticClass();
    DefaultPawnClass = AGoodBallPawn::StaticClass();
    PlayerControllerClass = AGoodBallPlayerController::StaticClass();
}

FString AGameJamModeBase::ConvertCurrentTimer() const
{
    if (this->TimeFromStart == 0) return(FString("00:00"));
    int32 Min = 0;
    int32 TempTime = this->TimeFromStart;
    
    while (TempTime >= 60)
    {
        Min++;
        TempTime -= 60;
    }
    int32 Sec = TempTime;
    FString TextMin = (Min == 0) ? "00" : FString::FromInt(Min);
    FString TextSec = (Sec >= 0 && Sec <= 9) ? "0" + FString::FromInt(Sec) : FString::FromInt(Sec);
    
    return (FString(TextMin + ":" + TextSec));
}

void AGameJamModeBase::StartPlay()
{
	Super::StartPlay();
	checkf(GetWorld(), TEXT("World is nullptr"));
	this->GameInst = Cast<UMSBJGameInstance>(GetGameInstance());
	checkf(this->GameInst, TEXT("Game instance is nullptr"));
	this->UserSettings = UGameUserSettings::GetGameUserSettings();
	checkf(this->UserSettings, TEXT("User settings is nullptr"));

	this->SetupStartLevelSettings();
	this->ChangeGameState(EGameLevelState::WaitToStart);
}

void AGameJamModeBase::ChangeGameState(EGameLevelState NewState)
{
	if (this->CurrentGameState == NewState)
		return;
    if (this->CurrentGameState == EGameLevelState::WaitToStart && NewState == EGameLevelState::InProgress)
    {
        GetWorld()->GetTimerManager().SetTimer(this->HandleUpTime, this, &AGameJamModeBase::IncrementTime, 1.f, true);
    }
    if (this->CurrentGameState == EGameLevelState::InProgress && NewState == EGameLevelState::GameOver)
    {
        GetWorld()->GetTimerManager().ClearTimer(this->HandleUpTime);
    }
	this->CurrentGameState = NewState;
	this->OnGameLevelStateChanged.Broadcast(NewState);
}

void AGameJamModeBase::SetupStartLevelSettings()
{
	UKismetInternationalizationLibrary::SetCurrentCulture(this->GameInst->GetCurrentLanguage(), false);
	this->UserSettings->SetFullscreenMode(this->GameInst->GetCurrentWindowMode());
	this->UserSettings->SetScreenResolution(this->GameInst->GetScreenViewport());
	this->UserSettings->SetAntiAliasingQuality(this->GameInst->GetCurrentQualityValue());
	this->UserSettings->SetAudioQualityLevel(this->GameInst->GetCurrentQualityValue());
	this->UserSettings->SetFoliageQuality(this->GameInst->GetCurrentQualityValue());
	this->UserSettings->SetPostProcessingQuality(this->GameInst->GetCurrentQualityValue());
	this->UserSettings->SetShadingQuality(this->GameInst->GetCurrentQualityValue());
	this->UserSettings->SetShadowQuality(this->GameInst->GetCurrentQualityValue());
	this->UserSettings->SetTextureQuality(this->GameInst->GetCurrentQualityValue());
	this->UserSettings->SetViewDistanceQuality(this->GameInst->GetCurrentQualityValue());
	this->UserSettings->SetVisualEffectQuality(this->GameInst->GetCurrentQualityValue());
	this->UserSettings->ApplySettings(false);
}

void AGameJamModeBase::IncrementTime()
{
    this->TimeFromStart++;
}
