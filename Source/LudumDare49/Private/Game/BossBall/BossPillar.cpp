// Fill out your copyright notice in the Description page of Project Settings.

#include "Game/BossBall/BossPillar.h"
#include "Engine/World.h"
#include "Game/BossBall/BossBallPawn.h"
#include "Kismet/GameplayStatics.h"

DEFINE_LOG_CATEGORY_STATIC(LogBossPillar, All, All)

// Sets default values
ABossPillar::ABossPillar()
{
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = false;

    CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComponent"));
    SetRootComponent(this->CapsuleComponent);
    Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
    Mesh->SetupAttachment(this->CapsuleComponent);

  //  CapsuleComponent->OnComponentBeginOverlap.AddDynamic(this, &ABossPillar::OnPillarBeginOverlap);
}

// Called when the game starts or when spawned
void ABossPillar::BeginPlay()
{
    Super::BeginPlay();
}

void ABossPillar::NotifyHit(UPrimitiveComponent* myComponent, AActor* other, UPrimitiveComponent* otherComp, bool selfMoved,
    FVector hitLocation, FVector hitNormal, FVector normalImpulse, const FHitResult& hitResult)
{
    Super::NotifyHit(myComponent, other, otherComp, selfMoved, hitLocation, hitNormal, normalImpulse, hitResult);

    if (other && other->IsA(ABossBallPawn::StaticClass()))
    {
        UE_LOG(LogBossPillar, Warning, TEXT("overlap: %s"), *other->GetName());
        GetWorldTimerManager().SetTimer(SetCollisionTimerHandle, this, &ABossPillar::SetCollisionDisabled, 0.25f, false);
    }
}

void ABossPillar::OnPillarBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
    int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
}

void ABossPillar::SetCollisionDisabled()
{
    UE_LOG(LogBossPillar, Warning, TEXT("SetCollision"));


    Mesh->SetMaterial(0, BaseMaterial);
    CanHit = false;
    this->CapsuleComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}
