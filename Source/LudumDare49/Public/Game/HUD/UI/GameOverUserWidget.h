// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Game/HUD/UI/BaseUserWidget.h"
#include "GameOverUserWidget.generated.h"

class UButton;
class UImage;
/**
 * 
 */
UCLASS()
class LUDUMDARE49_API UGameOverUserWidget : public UBaseUserWidget
{
    GENERATED_BODY()

protected:
    virtual void NativeOnInitialized() override;

    virtual void ShowAnimStart() override;

    UPROPERTY(Transient, meta = (BindWidget))
    UImage* BlackImage;
    UPROPERTY(Transient, meta = (BindWidget))
    UButton* MainMenuButton;

private:
    FTimerHandle TimerOnStart;

    UFUNCTION()
    void ClickButtonToMenu();

    void HideBlackImage();
};
