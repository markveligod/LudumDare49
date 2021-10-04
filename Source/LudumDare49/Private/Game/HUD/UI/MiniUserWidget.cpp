// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/HUD/UI/MiniUserWidget.h"

#include "Components/AudioComponent.h"
#include "Game/GameJamModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"

void UMiniUserWidget::StartTimerGame()
{
    GetWorld()->GetTimerManager().SetTimer(TimerHandleUp, this, &UMiniUserWidget::UpdateTime, this->RateTimeCallUp, true);
}

void UMiniUserWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();
    this->GameMode = Cast<AGameJamModeBase>(GetWorld()->GetAuthGameMode());
    check(this->GameMode);
    PlayAnimation(this->LoopAnim, 0, 0);
    PlaySound(this->StartMini);
    this->LoopMiniComponent = UGameplayStatics::SpawnSound2D(GetWorld(), this->LoopMini);
}

void UMiniUserWidget::UpdateTime()
{
    this->CurrentCount += 1;
    if (this->CurrentCount == this->MaxRunGameOver)
    {
        this->LoopMiniComponent->Stop();
        this->LoopMiniComponent->DestroyComponent();
        this->GameMode->ChangeGameState(EGameLevelState::GameOver);
        SetVisibility(ESlateVisibility::Hidden);
    }
}
