// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Menu/MSBJDataTypes.h"
#include "GoodBallPlayerController.generated.h"

class AGameJamModeBase;
/**
 * 
 */
UCLASS()
class LUDUMDARE49_API AGoodBallPlayerController : public APlayerController
{
    GENERATED_BODY()

protected:
    virtual void BeginPlay() override;

private:
    AGameJamModeBase* GameMode;

    void OnChangeGameState(EGameLevelState NewState);
};
