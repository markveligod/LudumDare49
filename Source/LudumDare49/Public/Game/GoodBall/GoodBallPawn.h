// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "GoodBallPawn.generated.h"

class USoundCue;
class AGameJamModeBase;
class UArrowComponent;
class UCameraComponent;
class USpringArmComponent;
UCLASS()
class LUDUMDARE49_API AGoodBallPawn : public APawn
{
    GENERATED_BODY()

public:
    // Sets default values for this pawn's properties
    AGoodBallPawn();

    // Called every frame
    virtual void Tick(float DeltaTime) override;

    // Called to bind functionality to input
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

    // Ball state
    bool StateMove = true;

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

    virtual void AddControllerYawInput(float Val) override;
    virtual void AddControllerPitchInput(float Val) override;

    virtual void AddMovementInput(FVector WorldDirection, float ScaleValue, bool bForce) override;

    // Receive notification of a collision contact and record that we're in contact with something.
    virtual void NotifyHit(UPrimitiveComponent* myComponent, AActor* other, UPrimitiveComponent* otherComp, bool selfMoved,
        FVector hitLocation, FVector hitNormal, FVector normalImpulse, const FHitResult& hitResult) override;

    // Is the ball bearing in contact with any other geometry?
    bool InContact = false;

    // The static mesh that represents the ball bearing.
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = BallBearing)
    UStaticMeshComponent* BallMesh = nullptr;

    // Spring arm for positioning the camera above the ball bearing.
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = BallBearing)
    USpringArmComponent* SpringArm = nullptr;

    // Camera to view the ball bearing.
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = BallBearing)
    UCameraComponent* Camera = nullptr;

    // How much force to use to push the ball bearing around.
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = BallBearing)
    float ControllerForce = 250.0f;

    // The maximum speed in meters per second.
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = BallBearing)
    float MaximumSpeed = 4.0f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = BallBearing)
    USoundCue* SpaceSound;
    
    FVector ForwardLocation;
    FVector RightLocation;

private:
    AGameJamModeBase* GameMode;

    void UpperKeyDrop();
    
    // Move the ball bearing with the given force longitudinally on the X axis.
    void MoveLongitudinally(float value)
    {
            this->AddMovementInput(ForwardLocation, value, true);
    }

    // Move the ball bearing with the given force longitudinally on the Y axis.
    void MoveLaterally(float value)
    {
            this->AddMovementInput(RightLocation, value, true);
    }

    // The current longitude input received from the player.
    float InputLongitude = 0.0f;

    // The current latitude input received from the player.
    float InputLatitude = 0.0f;

    // Update forward vector location
    void UpdateForwardLocation();

    // Update right vector location
    void UpdateRightLocation();

};
