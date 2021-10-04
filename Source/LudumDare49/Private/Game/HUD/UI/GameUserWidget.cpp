// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/HUD/UI/GameUserWidget.h"

#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"

void UGameUserWidget::ShowAnimEnd()
{
    UGameplayStatics::PlaySound2D(GetWorld(), this->WastedSound);
    Super::ShowAnimEnd();
}
