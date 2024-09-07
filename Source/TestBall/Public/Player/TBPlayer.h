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
class ATBAIPlayer;
class UTBPlayerAnimationComponent;
class UTBBallComputeDataComponent;
struct ShootingData;
struct PassingData;

UCLASS()
class TESTBALL_API ATBPlayer : public ACharacter
{
    GENERATED_BODY()

public:
    // Sets default values for this character's properties
    ATBPlayer();

    void MoveToBall();

    bool IsPlayerHaveBall() { return PlayerHaveBall; }
    void SetPlayerHaveBall(bool arg) { PlayerHaveBall = arg; }

    void SetRotationPlayerOnBall();
    UTBPlayerAnimationComponent* GetPlayerAnimationComponent() { return PlayerAnimationComponent; }
    bool IsReadyToShoot() { return PlayerReadyToShoot; }
    void SetReadyToShoot(bool arg) { PlayerReadyToShoot = arg; }

    ACage* GetOpponentGoalPost() { return OpponentGoalPost; }
    TArray<ATBAIPlayer*> GetTeam() { return Team; }

    float GetMaxDistanceToMoveTheBall() { return MaxDistanceToMoveTheBall; }

    bool IsCanMakePass();
    FVector FindVecMoveToPassBallPosition();

    bool IsCanMakeShoot();
    FVector FindVecMoveToShootBallPosition();

    void PassBall();
    bool IsPassAnimationExecuted();

    FVector GetVectorPlayerToBall();

    void ShootBall();
    bool IsShootAnimationExecuted();

    void TakeBall();
    bool IsCanTakeBall();
    bool IsTakeBallComplete();

    FVector GetBallLocation();
    bool IsMoveToBallComplete();

    ShootingData GetShootingData();
    PassingData GetPassingData();

    ABall1* GetBallPtr();

protected:
    virtual void BeginPlay() override;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
    USpringArmComponent* SpringArmComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
    UCameraComponent* CameraComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
    UTBPlayerAnimationComponent* PlayerAnimationComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
    UTBBallComputeDataComponent* BallComputeDataComponent;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Team")
    int32 NTeam = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GoalPosts")
    ACage* OwnGoalPost = nullptr;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GoalPosts")
    ACage* OpponentGoalPost = nullptr;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Team")
    TArray<ATBAIPlayer*> Team;

    UPROPERTY(EditDefaultsOnly, Category = "Animation")
    float MaxDistanceToMoveTheBall = 3000.0f;

    UPROPERTY(EditDefaultsOnly, Category = "Animation")
    float MaxDistanceToStartTakeBall = 800.0f;

    UPROPERTY(EditDefaultsOnly, Category = "Animation")
    float MinDistanceToStartTakeBall = 400.0f;

public:
    virtual void Tick(float DeltaTime) override;

private:
    void CheckMoveToBall();
    void MoveToBallAndShoot();

    void OnBallHit();

    bool IsMovingToBall     = false;
    bool PlayerReadyToShoot = false;

    bool bSetRotationPlayerOnBall = false;
    bool PlayerHaveBall           = false;
};
