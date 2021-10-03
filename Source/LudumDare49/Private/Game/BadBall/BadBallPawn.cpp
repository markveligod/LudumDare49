// Fill out your copyright notice in the Description page of Project Settings.

#include "Game/BadBall/BadBallPawn.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"

DEFINE_LOG_CATEGORY_STATIC(LogBadBall, All, All)

// Sets default values
ABadBallPawn::ABadBallPawn()
{
    // Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;
    Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
    Mesh->SetSimulatePhysics(true);
}

// Called when the game starts or when spawned
void ABadBallPawn::BeginPlay()
{
    Super::BeginPlay();

    StartMove();
}

void ABadBallPawn::StartMove()
{
    float FirstDelay = FMath::RandRange(MinFirstDelay, MaxFirstDelay);

    GetWorldTimerManager().SetTimer(MoveImpulseTimerHandle, this, &ABadBallPawn::MoveImpulse, MoveImpulseDelay, true, FirstDelay);
}

void ABadBallPawn::StopMove() {}

void ABadBallPawn::MoveImpulse()
{
    if (!this->StateMove) return;
    //////////////////// choosing forse ///////////////////////////////// choosing direction
    float XDirection = FMath::RandRange(MinMoveImpulse, MaxMoveImpulse) * FMath::RandRange(-1, 1);
    float YDirection = FMath::RandRange(MinMoveImpulse, MaxMoveImpulse) * FMath::RandRange(-1, 1);

    FVector ImpulseDirection(XDirection, YDirection, 0.0f);
    Mesh->AddImpulse(ImpulseDirection * MoveImpulseScale, NAME_None, true);
}

// Called every frame
void ABadBallPawn::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void ABadBallPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);
}
