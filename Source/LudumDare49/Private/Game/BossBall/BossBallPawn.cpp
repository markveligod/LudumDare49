// Fill out your copyright notice in the Description page of Project Settings.

#include "Game/BossBall/BossBallPawn.h"
#include "Game/BossBall/BossPillar.h"
#include "Game/BadBall/BadBallPawn.h"
#include "Game/Neutron/NeutronActor.h"
#include "Game/GoodBall/GoodBallPawn.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "Components/MeshComponent.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraFunctionLibrary.h"
#include "Sound/SoundCue.h"

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

void ABossBallPawn::NotifyHit(UPrimitiveComponent* myComponent, AActor* other, UPrimitiveComponent* otherComp, bool selfMoved,
    FVector hitLocation, FVector hitNormal, FVector normalImpulse, const FHitResult& hitResult)
{
    Super::NotifyHit(myComponent, other, otherComp, selfMoved, hitLocation, hitNormal, normalImpulse, hitResult);

    auto GoodBall = Cast<AGoodBallPawn>(other);
    if (GoodBall)
    {
        UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), this->BadBallDeathEffect, GoodBall->GetActorLocation());
        UNiagaraFunctionLibrary::SpawnSystemAtLocation(
            GetWorld(), this->BadBallDeathEffect, GoodBall->GetActorLocation() + FVector(0.f,0.f,50.f));
               UNiagaraFunctionLibrary::SpawnSystemAtLocation(
            GetWorld(), this->BadBallDeathEffect, GoodBall->GetActorLocation() + FVector(50.f, 0.f, 0.f));
        UNiagaraFunctionLibrary::SpawnSystemAtLocation(
            GetWorld(), this->BadBallDeathEffect, GoodBall->GetActorLocation() + FVector(0.f, 50.f, 0.f));

        GetWorldTimerManager().SetTimer(SetCollisionTimerHandle, this, &ABossBallPawn::Death, 0.25f, false);
        return;
    }

    auto BadBall = Cast<ABadBallPawn>(other);
    if (BadBall)
    {
        UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), this->BadBallDeathEffect, BadBall->GetActorLocation());
        BadBall->Destroy();
        return;
    }

    auto Pillar = Cast<ABossPillar>(other);
    if (!Pillar) return;
    if (!Pillar->CanHit) return;
    CurrentHealth--;
    UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), this->BadBallDeathEffect, this->GetActorLocation());


    UE_LOG(LogBossBall, Warning, TEXT("Health: %i"), CurrentHealth);

    UE_LOG(LogBossBall, Warning, TEXT("overlaped Actor: %s"), *Pillar->GetName());

    if (CurrentHealth <= 0)
    {
        auto Neutron = GetWorld()->SpawnActor<ANeutronActor>(NeutronActor);
        if (!Neutron) return;
        auto NeutronSpawnLocation = GetActorLocation();
        NeutronSpawnLocation.Z = 120.0f;
        Neutron->SetActorLocation(NeutronSpawnLocation);
        Neutron->SetNeutronCount(AmountOfNeutronsAfterDeath);
        GetWorldTimerManager().SetTimer(SetCollisionTimerHandle, this, &ABossBallPawn::Death, 0.25f, false);
    }
}

void ABossBallPawn::OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
    int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
}

void ABossBallPawn::MoveImpulse()
{

    auto PlayerPawn = Cast<AGoodBallPawn>(GetWorld()->GetFirstPlayerController()->GetPawn());

    if (!PlayerPawn) return;

    FVector ImpulseDirection;
    ImpulseDirection = PlayerPawn->GetActorLocation() - this->GetActorLocation();  // vector from BossBall to GoodBall

    if (ImpulseDirection.Size() > MaxDetectDistance) return;  // max range of detecting GoodBall

    if (this->StateMove == false)
    {
        this->Mesh->ComponentVelocity = FVector(0.f, 0.f, 0.f);
        return;
    }
    if (!this->StateMove) UE_LOG(LogBossBall, Warning, TEXT("StateMove FALSE"));

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
    UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), this->BossDeathEffect, GetActorLocation());
    UGameplayStatics::PlaySoundAtLocation(GetWorld(), this->DeathSound, GetActorLocation());
    this->Destroy();
}

// Called every frame
void ABossBallPawn::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}
