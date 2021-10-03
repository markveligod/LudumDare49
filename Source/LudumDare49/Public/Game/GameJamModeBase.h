// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Menu/MSBJDataTypes.h"
#include "GameJamModeBase.generated.h"

/**
 *
 */
UCLASS()
class LUDUMDARE49_API AGameJamModeBase : public AGameModeBase
{
    GENERATED_BODY()
public:
    AGameJamModeBase();

    void ChangeGameState(EGameLevelState NewState);

    FOnGameLevelStateChangedSignature OnGameLevelStateChanged;

    UFUNCTION(BlueprintCallable)
    FString ConvertCurrentTimer() const;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings game")
    int32 Neutrons = 100;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Settings game")
    int32 DecreaseCountNeutrons = 1;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Settings game")
    float RateTimeDecrease = 5.f;
    
protected:
    virtual void StartPlay() override;

private:
    class UMSBJGameInstance* GameInst;
    class UGameUserSettings* UserSettings;
    EGameLevelState CurrentGameState = EGameLevelState::WaitToStart;

    void SetupStartLevelSettings();

    int32 TimeFromStart = 0;
    FTimerHandle HandleUpTime;
    void IncrementTime();

    FTimerHandle HandleDecreaseTimer;
    void DecreaseCount();
};
