// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "BadBallPawn.generated.h"

UCLASS()
class LUDUMDARE49_API ABadBallPawn : public APawn
{
    GENERATED_BODY()

public:
    // Sets default values for this pawn's properties
    ABadBallPawn();

    // Called every frame
    virtual void Tick(float DeltaTime) override;

    // Called to bind functionality to input
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
    UStaticMeshComponent* Mesh = nullptr;

    UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Movement")
    float MinMoveImpulse = 1.0f;

    UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Movement")
    float MaxMoveImpulse = 3.0f;
    UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Movement")
    float MoveImpulseScale = 100.0f;

    UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Timer")
    float MoveImpulseDelay = 1.0f;

    UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Timer")
    float MinFirstDelay = 0.1f;

    UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Timer")
    float MaxFirstDelay = 0.5f;

private:
    void StartMove();
    void StopMove();  // calls when BadBall interact with GoodBall

    void MoveImpulse();

    FTimerHandle MoveImpulseTimerHandle;
};
