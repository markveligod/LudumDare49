// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/GameJamModeBase.h"

#include "Game/BadBall/BadBallPawn.h"
#include "Menu/MSBJGameInstance.h"
#include "GameFramework/GameUserSettings.h"
#include "Kismet/KismetInternationalizationLibrary.h"
#include "Game/HUD/GameHUD.h"
#include "Game/GoodBall/GoodBallPawn.h"
#include "Game/GoodBall/GoodBallPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Menu/HUD/UI/MSBJCreditsWidget.h"
#include "Game/HUD/UI/MiniUserWidget.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"

DEFINE_LOG_CATEGORY_STATIC(LogGameJamModeBase, All, All);

AGameJamModeBase::AGameJamModeBase()
{
    HUDClass = AGameHUD::StaticClass();
    DefaultPawnClass = AGoodBallPawn::StaticClass();
    PlayerControllerClass = AGoodBallPlayerController::StaticClass();
}

FString AGameJamModeBase::ConvertCurrentTimer() const
{
    if (this->TimeFromStart == 0) return (FString("00:00"));
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

void AGameJamModeBase::StartMini(ABadBallPawn* BadBallRef)
{
    for (auto TempBall : this->ArrayBadBalls)
        TempBall->StateMove = false;
    this->BadBallPointer = BadBallRef;
    this->BadBallPointer->Mesh->SetPhysicsLinearVelocity(FVector::ZeroVector);
    this->MaxKeyDrop += this->OnUpKeydrop;
    this->CurrentKeyDrop = 0;
    this->WidgetMiniPointer = CreateWidget<UMiniUserWidget>(GetWorld(), this->WidgetMini);
    this->WidgetMiniPointer->AddToViewport();
    this->WidgetMiniPointer->MaxRunGameOver = this->MaxRunGameOver;
    this->WidgetMiniPointer->RateTimeCallUp = this->RateTimeCallUp;
    this->WidgetMiniPointer->CurrentCount = 0;
    this->WidgetMiniPointer->StartTimerGame();
    this->StateMini = true;
}

void AGameJamModeBase::StopMini()
{
    GetWorld()->GetTimerManager().ClearTimer(this->WidgetMiniPointer->TimerHandleUp);
    this->WidgetMiniPointer->RemoveFromViewport();
    this->WidgetMiniPointer->Destruct();
    this->GoodBall->StateMove = true;
    this->MaxRunGameOver -= this->DecreaseCountMax;
    this->RateTimeCallUp -= this->DecreaseRunTime;
    this->StateMini = false;
    UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), this->DeathEffect, this->BadBallPointer->GetActorLocation());
    this->BadBallPointer->Destroy();
    
    for (auto TempBall : this->ArrayBadBalls)
        TempBall->StateMove = true;
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

    TArray<AActor*> TempArray;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABadBallPawn::StaticClass(), TempArray);
    for (auto TempActor : TempArray)
        if (TempActor->IsA(ABadBallPawn::StaticClass()))
            this->ArrayBadBalls.Add(Cast<ABadBallPawn>(TempActor));

    TempArray.Empty();
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABossBallPawn::StaticClass(), TempArray);
    for (auto TempActor : TempArray)
        if (TempActor->IsA(ABossBallPawn::StaticClass()))
            this->ArrayBoss.Add(Cast<ABossBallPawn>(TempActor));

    this->GoodBall = Cast<AGoodBallPawn>(UGameplayStatics::GetActorOfClass(GetWorld(), AGoodBallPawn::StaticClass()));
    check(this->GoodBall);

    this->MaxRunGameOver = this->StartMaxRunGameOver;
    this->RateTimeCallUp = this->StartRateTimeCallUp;

    this->ChangeGameState(EGameLevelState::WaitToStart);
}

void AGameJamModeBase::ChangeGameState(EGameLevelState NewState)
{
    UE_LOG(LogGameJamModeBase, Warning, TEXT("New State: %s"), *UEnum::GetValueAsString(NewState));
    if (NewState == EGameLevelState::WaitToStart || NewState == EGameLevelState::GameOver)
    {
        for (auto TempBadBall : this->ArrayBadBalls)
            TempBadBall->StateMove = false;
        for (auto TempBoss : this->ArrayBoss)
            TempBoss->StateMove = false;
        
    }
    if (NewState == EGameLevelState::GameOver && this->StateMini)
    {
        GetWorld()->GetTimerManager().ClearTimer(this->WidgetMiniPointer->TimerHandleUp);
        this->WidgetMiniPointer->RemoveFromViewport();
        this->WidgetMiniPointer->Destruct();
        this->StateMini = false;
    }
    if (this->CurrentGameState == EGameLevelState::WaitToStart && NewState == EGameLevelState::InProgress)
    {
        UE_LOG(LogGameJamModeBase, Warning, TEXT("New State: %s"), *UEnum::GetValueAsString(NewState));
        GetWorld()->GetTimerManager().SetTimer(this->HandleUpTime, this, &AGameJamModeBase::IncrementTime, 1.f, true);
        GetWorld()->GetTimerManager().SetTimer(this->HandleDecreaseTimer, this, &AGameJamModeBase::DecreaseCount, this->RateTimeDecrease,
            true);
        for (auto TempBadBall : this->ArrayBadBalls)
            TempBadBall->StateMove = true;
        for (auto TempBoss : this->ArrayBoss)
            TempBoss->StateMove = true;
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

void AGameJamModeBase::DecreaseCount()
{
    if (this->StateMini) return;
    this->Neutrons -= this->DecreaseCountNeutrons;
    if (this->Neutrons <= 0)
    {
        GetWorld()->GetTimerManager().ClearTimer(this->HandleDecreaseTimer);
        this->ChangeGameState(EGameLevelState::GameOver);
    }
}
