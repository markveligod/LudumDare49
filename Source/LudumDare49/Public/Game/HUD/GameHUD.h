// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "Menu/MSBJDataTypes.h"
#include "GameHUD.generated.h"

class AGameJamModeBase;
class UBaseUserWidget;
/**
 * 
 */
UCLASS()
class LUDUMDARE49_API AGameHUD : public AHUD
{
    GENERATED_BODY()

public:
    // A set of sample widgets to add to the game
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
    TSubclassOf<UBaseUserWidget> GameProgressWidgetClass;
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
    TSubclassOf<UBaseUserWidget> WelcomeWidgetClass;
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
    TSubclassOf<UBaseUserWidget> GameOverWidgetClass;

protected:
    virtual void BeginPlay() override;

    
private:
    AGameJamModeBase* GameMode;

    // Map for storing all widgets
    TMap<EGameLevelState, UBaseUserWidget*> GameWidgets;
    UBaseUserWidget* WidgetOnVisible = nullptr;

    EGameLevelState GameState = EGameLevelState::WaitToStart;

    // function for signing on to delegate changes to the gameplay state
    void OnChangedGameState(EGameLevelState NewState);

    FTimerHandle TimerHandleSwitch;

    // Reset end animation
    void ResetToEndAnimation();
    // Reset start animation
    void ResetToStartAnim();

    // Setup new widget menu
    void SetupWidgetMenu();
};
