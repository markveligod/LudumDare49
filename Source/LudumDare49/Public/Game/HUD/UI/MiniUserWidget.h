// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MiniUserWidget.generated.h"

class USoundCue;
class AGameJamModeBase;
/**
 * 
 */
UCLASS()
class LUDUMDARE49_API UMiniUserWidget : public UUserWidget
{
    GENERATED_BODY()
public:
    void StartTimerGame();

    float RateTimeCallUp = 1.f;
    UPROPERTY(BlueprintReadOnly)
    int32 MaxRunGameOver = 100;
    UPROPERTY(BlueprintReadOnly)
    int32 CurrentCount = 0;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Sounds")
    USoundCue* StartMini;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Sounds")
    USoundCue* LoopMini;

    UAudioComponent* LoopMiniComponent;
    
    FTimerHandle TimerHandleUp;
protected:
    virtual void NativeOnInitialized() override;

    UPROPERTY(Transient, meta = (BindWidgetAnim))
    UWidgetAnimation* LoopAnim;

private:
    AGameJamModeBase* GameMode;

    void UpdateTime();
};
