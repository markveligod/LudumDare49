// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/Neutron/NeutronActor.h"
#include "Components/SphereComponent.h"
#include "Game/GameJamModeBase.h"
#include "Game/GoodBall/GoodBallPawn.h"

// Sets default values
ANeutronActor::ANeutronActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    // Create Default Scene root
    this->SceneRoot = CreateDefaultSubobject<USceneComponent>("Root Scene");
    SetRootComponent(this->SceneRoot);

    // Create Base Mesh
    this->BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>("Base mesh");
    this->BaseMesh->SetupAttachment(this->SceneRoot);
    // Create First Mesh
    this->FirstMesh = CreateDefaultSubobject<UStaticMeshComponent>("First mesh");
    this->FirstMesh->SetupAttachment(this->BaseMesh);
    // Create Second Mesh
    this->SecondMesh = CreateDefaultSubobject<UStaticMeshComponent>("Second mesh");
    this->SecondMesh->SetupAttachment(this->BaseMesh);
    // Create Third Mesh
    this->ThirdMesh = CreateDefaultSubobject<UStaticMeshComponent>("Third mesh");
    this->ThirdMesh->SetupAttachment(this->BaseMesh);

    //Create Sphere Collision
    this->SphereCollision = CreateDefaultSubobject<USphereComponent>("Sphere collision");
    this->SphereCollision->SetupAttachment(this->SceneRoot);
    
}

// Called when the game starts or when spawned
void ANeutronActor::BeginPlay()
{
	Super::BeginPlay();

    this->SphereCollision->OnComponentBeginOverlap.AddDynamic(this, &ANeutronActor::RegisterBeginOverlapCollision);
}

void ANeutronActor::RegisterBeginOverlapCollision(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (OtherActor->IsA(AGoodBallPawn::StaticClass()))
    {
        auto GameMode = Cast<AGameJamModeBase>(GetWorld()->GetAuthGameMode());
        GameMode->Neutrons += this->CountNeutron;
        Destroy();
    }
}

// Called every frame
void ANeutronActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
    this->FirstMesh->AddRelativeRotation(FRotator(1.f, 0.f, 0.f));
    this->SecondMesh->AddRelativeRotation(FRotator( 1.f, 1.f, 0.f));
    this->ThirdMesh->AddRelativeRotation(FRotator(0.f, 0.f, 1.f));

}

