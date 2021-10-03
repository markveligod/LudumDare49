// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/CapsuleComponent.h"
#include "BossPillar.generated.h"

UCapsuleComponent;

UCLASS()
class LUDUMDARE49_API ABossPillar : public AActor
{
    GENERATED_BODY()

public:
    // Sets default values for this actor's properties
    ABossPillar();

    void OnPillarHit();

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
    UStaticMeshComponent* Mesh = nullptr;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
    UCapsuleComponent* CapsuleComponent = nullptr;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bGotHit;
};
