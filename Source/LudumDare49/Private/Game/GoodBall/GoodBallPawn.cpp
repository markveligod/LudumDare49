// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/GoodBall/GoodBallPawn.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/ArrowComponent.h"
#include "GameFramework/PlayerInput.h"
#include "DrawDebugHelpers.h"
#include "Game/BadBall/BadBallPawn.h"
#include "Game/GameJamModeBase.h"

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

    this->GameMode = Cast<AGameJamModeBase>(GetWorld()->GetAuthGameMode());
    check(this->GameMode);
}

void AGoodBallPawn::AddControllerYawInput(float Val)
{
    if (!this->StateMove) return;
    Super::AddControllerYawInput(Val);
    FRotator TempRot = this->SpringArm->GetRelativeRotation();
    TempRot.Yaw += Val;
    TempRot.Roll = 0.f;
    this->SpringArm->SetRelativeRotation(TempRot);
}

void AGoodBallPawn::AddControllerPitchInput(float Val)
{
    if (!this->StateMove) return;
    Super::AddControllerPitchInput(Val);
    FRotator TempRot = this->SpringArm->GetRelativeRotation();
    TempRot.Pitch += Val;
    TempRot.Roll = 0.f;
    this->SpringArm->SetRelativeRotation(TempRot);
}

void AGoodBallPawn::AddMovementInput(FVector WorldDirection, float ScaleValue, bool bForce)
{
    Super::AddMovementInput(WorldDirection, ScaleValue, bForce);

    if (!StateMove) return;
    
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
        WorldDirection.X *= ScaleValue;
        WorldDirection.Y *= ScaleValue;
        WorldDirection.Z = 0;
        this->BallMesh->AddForce(WorldDirection * ControllerForce * BallMesh->GetMass());
    }
}

void AGoodBallPawn::NotifyHit(UPrimitiveComponent* MyComponent, AActor* Other, UPrimitiveComponent* OtherComp, bool bSelfMoved,
    FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& HitResult)
{
    Super::NotifyHit(MyComponent, Other, OtherComp, bSelfMoved, HitLocation, HitNormal, NormalImpulse, HitResult);
    if (Other && Other->IsA(ABadBallPawn::StaticClass()) && this->StateMove == true)
    {
        this->StateMove = false;
        this->BallMesh->SetPhysicsLinearVelocity(FVector::ZeroVector);
        this->GameMode->StartMini(Cast<ABadBallPawn>(Other));
    }
    
    InContact = true;
}

void AGoodBallPawn::UpperKeyDrop()
{
    if (this->GameMode->StateMini)
    {
        this->GameMode->CurrentKeyDrop += 1;
        if (this->GameMode->CurrentKeyDrop == this->GameMode->MaxKeyDrop)
            this->GameMode->StopMini();
    }
}

void AGoodBallPawn::UpdateForwardLocation()
{
    FVector StartPos = this->BallMesh->GetComponentLocation();
    FRotator TempRot = this->SpringArm->GetRelativeRotation();
    TempRot.Pitch = 0.f;
    TempRot.Roll = 0.f;
    FVector EndPos = StartPos + TempRot.Vector() * 300.f;

    ForwardLocation = (EndPos - StartPos);
    ForwardLocation.Normalize();
}

void AGoodBallPawn::UpdateRightLocation()
{
    FVector StartPos = this->BallMesh->GetComponentLocation();
    FRotator TempRot = this->SpringArm->GetRelativeRotation();
    TempRot.Pitch = 0.f;
    TempRot.Yaw += 90.f;
    TempRot.Roll = 0.f;
    FVector EndPos = StartPos + TempRot.Vector() * 300.f;

    RightLocation = (EndPos - StartPos);
    RightLocation.Normalize();
}

// Called every frame
void AGoodBallPawn::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    InContact = false;
    this->UpdateForwardLocation();
    DrawDebugLine(GetWorld(), GetActorLocation(), GetActorLocation() + ForwardLocation * 300.f, FColor::Red, false, -1, 0, 5);
    this->UpdateRightLocation();
    DrawDebugLine(GetWorld(), GetActorLocation(), GetActorLocation() + RightLocation * 300.f, FColor::Blue, false, -1, 0, 5);
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
        UPlayerInput::AddEngineDefinedAxisMapping(FInputAxisKeyMapping("GoodBall_Turn", EKeys::MouseX, 1.f));
        UPlayerInput::AddEngineDefinedAxisMapping(FInputAxisKeyMapping("GoodBall_LookUp", EKeys::MouseY, 1.f));
        UPlayerInput::AddEngineDefinedActionMapping(FInputActionKeyMapping("UpperMini", EKeys::SpaceBar));
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
        PlayerInputComponent->BindAxis("GoodBall_Turn", this, &AGoodBallPawn::AddControllerYawInput);
        PlayerInputComponent->BindAxis("GoodBall_LookUp", this, &AGoodBallPawn::AddControllerPitchInput);
        PlayerInputComponent->BindAction("UpperMini", EInputEvent::IE_Pressed, this, &AGoodBallPawn::UpperKeyDrop);
    }
}
