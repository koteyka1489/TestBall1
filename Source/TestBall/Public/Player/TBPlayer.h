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
class ATBAIPlayer;
class UTBPlayerAnimationComponent;

struct ShootingData
{
    FVector ShootingDirection;
    FVector ShootingRotation;
};

struct PassingData
{
    FVector PassDirection;
    FVector PassRotation;
};

USTRUCT(BlueprintType)
struct FShootingRandoms
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shooting")
    float x = 500.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shooting")
    float y = 100.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shooting")
    float zMin = 300.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shooting")
    float zMax = 1000.0f;
};

UCLASS()
class TESTBALL_API ATBPlayer : public ACharacter
{
    GENERATED_BODY()

public:
    // Sets default values for this character's properties
    ATBPlayer();

    UFUNCTION(BlueprintCallable, Category = "Ball")
    FVector GetBallLocation();

    UFUNCTION(BlueprintCallable, Category = "Ball")
    float GetDistanceToBall();

    UFUNCTION(BlueprintCallable, Category = "Ball")
    ABall1* GetBallPtr() { return Ball; }

    FVector FindVecMoveToShootBallPosition();
    FVector FindVecMoveToPassBallPosition();

    ShootingData GetShootingData();
    PassingData GetPassingData();

    void MoveToBall();

    bool IsPlayerHaveBall() { return PlayerHaveBall; }
    void SetPlayerHaveBall(bool arg) { PlayerHaveBall = arg; }
    void SetRotationPlayerOnBall();
    UTBPlayerAnimationComponent* GetPlayerAnimationComponent() { return PlayerAnimationComponent; }
    bool IsReadyToShoot() { return PlayerReadyToShoot; }
    void SetReadyToShoot(bool arg) { PlayerReadyToShoot = arg; }

protected:
    virtual void BeginPlay() override;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
    USpringArmComponent* SpringArmComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
    UCameraComponent* CameraComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
    UTBPlayerAnimationComponent* PlayerAnimationComponent;

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

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shooting")
    FShootingRandoms ShootingRandoms;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Team")
    TArray<ATBAIPlayer*> Team;

    UPROPERTY(EditDefaultsOnly, Category = "Animation")
    float MaxDistanceToMoveTheBall = 3000.0f;

public:
    virtual void Tick(float DeltaTime) override;
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
    void MoveForward(float Amount);
    void MoveRight(float Amount);
    void CheckBallLocationAndDirection();
    void CheckPlayerToBallDirection();
    void CheckBallLocation();

    void CheckMoveToBall();
    void MoveToBallAndShoot();

    void OnBallHit();
    

    bool IsMovingToBall           = false;
    bool PlayerReadyToShoot       = false;
    bool BallIsForward            = false;
    bool BallIsCloseLocation      = false;
    bool bSetRotationPlayerOnBall = false;
    bool PlayerHaveBall           = false;

    FVector VectorToBall = FVector::ZeroVector;
};
