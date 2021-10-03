// Fill out your copyright notice in the Description page of Project Settings.

#include "Game/BossBall/BossBallPawn.h"
#include "Game/BossBall/BossPillar.h"
#include "Game/Neutron/NeutronActor.h"
#include "Game/GoodBall/GoodBallPawn.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "Components/MeshComponent.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"

DEFINE_LOG_CATEGORY_STATIC(LogBossBall, All, All)

// Sets default values
ABossBallPawn::ABossBallPawn()
{
    // Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
    Mesh->SetSimulatePhysics(true);

    Mesh->OnComponentBeginOverlap.AddDynamic(this, &ABossBallPawn::OnSphereBeginOverlap);
}

// Called when the game starts or when spawned
void ABossBallPawn::BeginPlay()
{
    Super::BeginPlay();

    GetWorldTimerManager().SetTimer(
        MoveImpulseTimerHandle, this, &ABossBallPawn::MoveImpulse, MoveImpulseDelay, true, FMath::RandRange(0.1f, 0.5f));
}

void ABossBallPawn::OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
    int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
   // if (OtherActor) UE_LOG(LogBossBall, Warning, TEXT("overlap: %s"), *OtherActor->GetName());

    auto Pillar = Cast<ABossPillar>(OtherActor);
    if (!Pillar) return;

    CurrentHealth--;
    UE_LOG(LogBossBall, Warning, TEXT("overlap: %i"), CurrentHealth);
    if (CurrentHealth <= 0)
    {

        auto Neutron = GetWorld()->SpawnActor<ANeutronActor>(NeutronActor);
        if (!Neutron) return;
        auto NeutronSpawnLocation = GetActorLocation();
        Neutron->SetActorLocation(NeutronSpawnLocation);
        Neutron->SetNeutronCount(AmountOfNeutronsAfterDeath);

        Death();
    }
}

void ABossBallPawn::MoveImpulse()
{

    auto PlayerPawn = Cast<AGoodBallPawn>(GetWorld()->GetFirstPlayerController()->GetPawn());

    if (!PlayerPawn) return;

    FVector ImpulseDirection;
    ImpulseDirection = PlayerPawn->GetActorLocation() - this->GetActorLocation();  // vector from BossBall to GoodBall

    if (ImpulseDirection.Size() > MaxDetectDistance) return;  // max range of detecting GoodBall

    // add random deviation of impulse to base vector
    ImpulseDirection = ImpulseDirection + MoveImpulseScale * FVector(                                                         //
                                                                 FMath::RandRange(MinDeviationImpulse, MaxDeviationImpulse),  // X
                                                                 FMath::RandRange(MinDeviationImpulse, MaxDeviationImpulse),  // Y
                                                                 0.0f)
                                                                 .Normalize();  // Z

    Mesh->AddImpulse(ImpulseDirection, NAME_None, true);
}

void ABossBallPawn::Death()
{
    this->Destroy();
}

// Called every frame
void ABossBallPawn::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}
