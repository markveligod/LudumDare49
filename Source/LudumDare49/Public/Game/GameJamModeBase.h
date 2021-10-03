// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NiagaraSystem.h"
#include "BossBall/BossBallPawn.h"
#include "GameFramework/GameModeBase.h"
#include "Menu/MSBJDataTypes.h"
#include "GameJamModeBase.generated.h"

class UNiagaraComponent;
class UMiniUserWidget;
class AGoodBallPawn;
class ABadBallPawn;
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

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings game")
    int32 Neutrons = 100;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Settings game")
    int32 DecreaseCountNeutrons = 1;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Settings game")
    float RateTimeDecrease = 5.f;

    void StartMini(ABadBallPawn* BadBallRef);
    void StopMini();
    bool StateMini = false;
    TArray<ABadBallPawn*> ArrayBadBalls;
    ABadBallPawn* BadBallPointer;

    UMiniUserWidget* WidgetMiniPointer;

    UPROPERTY(BlueprintReadOnly)
    int32 MaxKeyDrop = 20;
    UPROPERTY(BlueprintReadOnly)
    int32 CurrentKeyDrop = 0;
    float RateTimeCallUp;
    int32 MaxRunGameOver;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Settings Mini loli-pop")
    TSubclassOf<UMiniUserWidget> WidgetMini;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Settings Mini loli-pop")
    int32 OnUpKeydrop = 5;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Settings Mini loli-pop")
    int32 StartMaxRunGameOver = 20;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Settings Mini loli-pop")
    float StartRateTimeCallUp = 1.f;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Settings Mini loli-pop")
    float DecreaseRunTime = 0.05f;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Settings Mini loli-pop")
    int32 DecreaseCountMax = 5;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components")
    UNiagaraSystem* DeathEffect;
protected:
    virtual void StartPlay() override;

    

private:
    TArray<ABossBallPawn*> ArrayBoss;
    class UMSBJGameInstance* GameInst;
    class UGameUserSettings* UserSettings;
    EGameLevelState CurrentGameState = EGameLevelState::WaitToStart;

    AGoodBallPawn* GoodBall;

    void SetupStartLevelSettings();

    int32 TimeFromStart = 0;
    FTimerHandle HandleUpTime;
    void IncrementTime();

    FTimerHandle HandleDecreaseTimer;
    void DecreaseCount();
};
