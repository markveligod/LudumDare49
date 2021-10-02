// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/GoodBall/GoodBallPawn.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/PlayerInput.h"

// Sets default values
AGoodBallPawn::AGoodBallPawn()
{
    // Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    // Create the ball mesh, turn on its physics and set it as the root component.
    this->BallMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BallMesh"));
    this->BallMesh->SetSimulatePhysics(true);
    SetRootComponent(BallMesh);

    // Create a spring-arm attached to the ball mesh.
    this->SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
    this->SpringArm->bDoCollisionTest = false;
    this->SpringArm->SetUsingAbsoluteRotation(true);
    this->SpringArm->SetRelativeRotation(FRotator(-45.0f, 0.0f, 0.0f));
    this->SpringArm->TargetArmLength = 1000.0f;
    this->SpringArm->bEnableCameraLag = false;
    this->SpringArm->CameraLagSpeed = 5.0f;
    this->SpringArm->SetupAttachment(BallMesh);

    // Create a camera and attach to the spring-arm.
    this->Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
    this->Camera->bUsePawnControlRotation = false;
    this->Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);
}

// Called when the game starts or when spawned
void AGoodBallPawn::BeginPlay()
{
    Super::BeginPlay();
    this->BallMesh->SetLinearDamping(0.5f);
    this->BallMesh->SetAngularDamping(0.5f);
}

void AGoodBallPawn::UpdateMoveBall()
{
    FVector velocity = BallMesh->GetPhysicsLinearVelocity();
    float z = velocity.Z;

    velocity.Z = 0.0f;

    if (velocity.Size() > MaximumSpeed * 100.0f)
    {
        velocity.Normalize();
        velocity *= MaximumSpeed * 100.0f;
        velocity.Z = z;

        FVector mergedVelocity = FMath::Lerp(BallMesh->GetPhysicsLinearVelocity(), velocity, 1.f);

        this->BallMesh->SetPhysicsLinearVelocity(mergedVelocity);
    }
    else
    {
        this->BallMesh->AddForce(FVector(InputLongitude, InputLatitude, 0.0f) * ControllerForce * BallMesh->GetMass());
    }
}

// Called every frame
void AGoodBallPawn::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    InContact = false;
    this->UpdateMoveBall();
}

// Setup Key for bind
static void InitializeDefaultPawnInputBindings()
{
    static bool bindingsAdded = false;

    if (bindingsAdded == false)
    {
        bindingsAdded = true;

        UPlayerInput::AddEngineDefinedAxisMapping(FInputAxisKeyMapping("GoodBall_MoveLongitudinally", EKeys::W, 1.f));
        UPlayerInput::AddEngineDefinedAxisMapping(FInputAxisKeyMapping("GoodBall_MoveLongitudinally", EKeys::S, -1.f));
        UPlayerInput::AddEngineDefinedAxisMapping(FInputAxisKeyMapping("GoodBall_MoveLaterally", EKeys::A, -1.f));
        UPlayerInput::AddEngineDefinedAxisMapping(FInputAxisKeyMapping("GoodBall_MoveLaterally", EKeys::D, 1.f));
    }
}

// Called to bind functionality to input
void AGoodBallPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);
    InitializeDefaultPawnInputBindings();
    if (PlayerInputComponent)
    {
        PlayerInputComponent->BindAxis("GoodBall_MoveLongitudinally", this, &AGoodBallPawn::MoveLongitudinally);
        PlayerInputComponent->BindAxis("GoodBall_MoveLaterally", this, &AGoodBallPawn::MoveLaterally);
    }
}
