// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"

#include "TBPlayer.generated.h"

class UCameraComponent;
class USpringArmComponent;
class USphereComponent;
class ABall1;
class UAnimmontage;
class ACage;
class ABall1;

struct ShootingData
{
    FVector ShootingDirection;
    FVector ShootingRotation;

};



UCLASS()
class TESTBALL_API ATBPlayer : public ACharacter
{
    GENERATED_BODY()

public:
    // Sets default values for this character's properties
    ATBPlayer();

    UFUNCTION(BlueprintCallable)
    bool IsRedyToShoot() { return ReadyToShoot; }

    UFUNCTION(BlueprintCallable, Category = "Ball")
    FVector GetBallLocation();

     UFUNCTION(BlueprintCallable, Category = "Ball")
    float GetDistanceToBall();

    FVector FindVecMoveToShootBallPosition();

    float GetShootTheBallDistance() { return ShootTheBallDistance; }

    
    ShootingData GetShootingData();

    virtual bool Shoot(float VecToBallLenght);
    void MoveToBall();
    bool IsShootAnimationExecuted() { return ShootAnimationExecuted; }

protected:
    virtual void BeginPlay() override;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
    USpringArmComponent* SpringArmComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
    UCameraComponent* CameraComponent;

    UPROPERTY(EditDefaultsOnly, Category = "Animation")
    UAnimMontage* ShotAnimMontage;

    UPROPERTY(EditDefaultsOnly, Category = "Animation")
    float ShootTheBallDistance = 135.0f;

    UPROPERTY(EditDefaultsOnly, Category = "Animation")
    float MaxDistanceToMoveTheBall = 3000.0f;

    UPROPERTY(EditDefaultsOnly, Category = "Animation")
    bool LockCameraOnShoot = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Team")
    int32 NTeam = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GoalPosts")
    ACage* OwnGoalPost = nullptr;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GoalPosts")
    ACage* OpponentGoalPost = nullptr;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ball")
    ABall1* Ball = nullptr;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shooting")
    float ShootingStrench = 5000;

   

public:
    virtual void Tick(float DeltaTime) override;

    // Called to bind functionality to input
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
    void MoveForward(float Amount);
    void MoveRight(float Amount);
    void CheckBallLocationAndDirection();
    void CheckPlayerToBallDirection();
    void CheckBallLocation();
    
    void InitAnimationNotify();
    void OnShootAnimationFinished();
    void CheckMoveToBall();
    void MoveToBallAndShoot();
    
    void OnBallHit();
    void LockCamera();

    bool IsMovingToBall      = false;
    bool BallIsForward       = false;
    bool BallIsCloseLocation = false;
    bool ReadyToShoot        = false;
    bool ShootAnimationExecuted     = false;
    FVector VectorToBall     = FVector::ZeroVector;
};
