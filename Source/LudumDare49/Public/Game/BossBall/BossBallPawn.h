// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Components/SphereComponent.h"
#include "Game/Neutron/NeutronActor.h"

#include "BossBallPawn.generated.h"

USphereComponent;
ANeutronActor;

class UNiagaraSystem;

UCLASS()
class LUDUMDARE49_API ABossBallPawn : public APawn
{
    GENERATED_BODY()

public:
    // Sets default values for this pawn's properties
    ABossBallPawn();

    // Called every frame
    virtual void Tick(float DeltaTime) override;

    bool StateMove = true;

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Niagara")
    UNiagaraSystem* BossDeathEffect;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Niagara")
    UNiagaraSystem* BadBallDeathEffect;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
    UStaticMeshComponent* Mesh = nullptr;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
    TSubclassOf<ANeutronActor> NeutronActor;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
    USphereComponent* SphereComponent = nullptr;

    UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Movement")
    float MinDeviationImpulse = -0.30f;

    UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Movement")
    float MaxDeviationImpulse = 0.30f;

    UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Movement")
    float MoveImpulseScale = 100.0f;

    UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Timer")
    float MoveImpulseDelay = 1.0f;

    UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Movement")
    float MaxDetectDistance = 10000.0f;

    UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Health")
    int32 MaxHealth = 3;

    UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Neutrons")
    int32 AmountOfNeutronsAfterDeath = 15;

    UFUNCTION()
    void OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
        bool bFromSweep, const FHitResult& SweepResult);

private:
    void MoveImpulse();

    int32 CurrentHealth = 3;
    void Death();

    FTimerHandle MoveImpulseTimerHandle;
};
