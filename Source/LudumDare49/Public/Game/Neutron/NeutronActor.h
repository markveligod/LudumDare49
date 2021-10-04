// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NeutronActor.generated.h"

class USoundCue;
class USphereComponent;
UCLASS()
class LUDUMDARE49_API ANeutronActor : public AActor
{
    GENERATED_BODY()

public:
    // Sets default values for this actor's properties
    ANeutronActor();

    // need for setting the number of spawned neutrons
    void SetNeutronCount(int32 Neutrons) { CountNeutron = Neutrons; };

    // Called every frame
    virtual void Tick(float DeltaTime) override;

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Components)
    USceneComponent* SceneRoot;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Components)
    UStaticMeshComponent* BaseMesh;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Components)
    UStaticMeshComponent* FirstMesh;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Components)
    UStaticMeshComponent* SecondMesh;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Components)
    UStaticMeshComponent* ThirdMesh;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Components)
    USphereComponent* SphereCollision;

    UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Settings Neutron")
    int32 CountNeutron = 5;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Sounds")
    USoundCue* PickUpSound;

private:
    UFUNCTION()
    void RegisterBeginOverlapCollision(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
        int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
