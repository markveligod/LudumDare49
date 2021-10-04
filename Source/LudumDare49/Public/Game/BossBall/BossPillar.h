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

    bool CanHit =true;

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
    UStaticMeshComponent* Mesh = nullptr;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
    UCapsuleComponent* CapsuleComponent = nullptr;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
    UMaterialInterface* GlowMaterial;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
    UMaterialInterface* BaseMaterial;




    virtual void NotifyHit(UPrimitiveComponent* myComponent, AActor* other, UPrimitiveComponent* otherComp, bool selfMoved,
        FVector hitLocation, FVector hitNormal, FVector normalImpulse, const FHitResult& hitResult) override;

private:
    UFUNCTION()
    void OnPillarBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
        bool bFromSweep, const FHitResult& SweepResult);

    FTimerHandle SetCollisionTimerHandle;

    void SetCollisionDisabled();
};
