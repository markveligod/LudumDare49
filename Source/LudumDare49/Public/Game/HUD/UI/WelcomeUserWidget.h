// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Game/HUD/UI/BaseUserWidget.h"
#include "WelcomeUserWidget.generated.h"

class UImage;
class UButton;
/**
 * 
 */
UCLASS()
class LUDUMDARE49_API UWelcomeUserWidget : public UBaseUserWidget
{
    GENERATED_BODY()

protected:
    virtual void NativeOnInitialized() override;

    UPROPERTY(Transient, meta = (BindWidget))
    UButton* ContinueButton;

    UPROPERTY(Transient, meta = (BindWidget))
    UImage* BlackImage;

    virtual void ShowAnimStart() override;
    virtual void ShowAnimEnd() override;

private:
    UFUNCTION()
    void ChangeGameState();

    FTimerHandle TimerDelay;

    void ResetTimer();
};


