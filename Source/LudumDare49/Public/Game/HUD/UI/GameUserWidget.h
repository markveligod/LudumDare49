// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Game/HUD/UI/BaseUserWidget.h"
#include "GameUserWidget.generated.h"

class USoundCue;

/**
 * 
 */
UCLASS()
class LUDUMDARE49_API UGameUserWidget : public UBaseUserWidget
{
	GENERATED_BODY()
	protected:
    virtual void ShowAnimEnd() override;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Sounds")
    USoundCue* WastedSound;
};
