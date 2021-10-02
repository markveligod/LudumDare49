// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "GoodBallPawn.generated.h"

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

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

    // Receive notification of a collision contact and record that we're in contact with something.
    virtual void NotifyHit(UPrimitiveComponent* myComponent, AActor* other, UPrimitiveComponent* otherComp, bool selfMoved,
        FVector hitLocation, FVector hitNormal, FVector normalImpulse, const FHitResult& hitResult) override
    {
        Super::NotifyHit(myComponent, other, otherComp, selfMoved, hitLocation, hitNormal, normalImpulse, hitResult);

        InContact = true;
    }

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

private:
    // Move the ball bearing with the given force longitudinally on the X axis.
    void MoveLongitudinally(float value)
    {
        InputLongitude = value;
    }

    // Move the ball bearing with the given force longitudinally on the Y axis.
    void MoveLaterally(float value)
    {
        InputLatitude = value;
    }

    // The current longitude input received from the player.
    float InputLongitude = 0.0f;

    // The current latitude input received from the player.
    float InputLatitude = 0.0f;

    // Update Move good ball
    void UpdateMoveBall();
};
