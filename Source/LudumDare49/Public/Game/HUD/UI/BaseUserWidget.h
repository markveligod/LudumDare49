// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BaseUserWidget.generated.h"

class AGameJamModeBase;

UENUM(BlueprintType)
enum class EStateAnimPlay : uint8
{
    None = 0,
    StartAnim,
    EndAnim
};

/**
 * 
 */
UCLASS()
class LUDUMDARE49_API UBaseUserWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Editor Timer")
    TMap<EStateAnimPlay, float> MapTimeOutRate;

    UFUNCTION()
    virtual void ShowAnimStart() { PlayAnimation(StartAnim); }

    UFUNCTION()
    virtual void ShowAnimEnd() { PlayAnimation(EndAnim); }

protected:
    // Getting current game mode
    AGameJamModeBase* GetGameMode() { return (this->GameMode); }

    // Native init
    virtual void NativeOnInitialized() override;

    UPROPERTY(Transient, meta = (BindWidgetAnim))
    UWidgetAnimation* StartAnim;
    UPROPERTY(Transient, meta = (BindWidgetAnim))
    UWidgetAnimation* EndAnim;
private:
    // Pointer on Game mode
    AGameJamModeBase* GameMode;
};
