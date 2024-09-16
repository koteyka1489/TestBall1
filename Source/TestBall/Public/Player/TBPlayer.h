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
class UTBPlayerAnimationComponent;
class UTBBallComputeDataComponent;
struct ShootingData;
struct PassingData;
class ATBAIController;
class UTBPlayerStateComponent;
enum class EPlayerState : uint8;
class UTBBrainComponent;
class UTBTextRenderComponent;

namespace EPathFollowingResult
{
enum Type : int;
}

UCLASS()
class TESTBALL_API ATBPlayer : public ACharacter
{
    GENERATED_BODY()

public:
    // Sets default values for this character's properties
    ATBPlayer();
    virtual void Tick(float DeltaTime) override;

    UTBPlayerAnimationComponent* GetPlayerAnimationComponent() { return PlayerAnimationComponent; }
    UTBBrainComponent* GetBrainComponent() { return BrainComponent; }
    UTBBallComputeDataComponent* GetBallComputeDataComponent() { return BallComputeDataComponent; }
    UTBPlayerStateComponent* GetPlayerStateComponent() { return PlayerStateComponent; }
    ABall1* GetBallPtr();

    bool IsPlayerHaveBall() const { return PlayerHaveBall; }
    void SetPlayerHaveBall(bool arg) { PlayerHaveBall = arg; }

    float GetMaxDistanceToMoveTheBall() const { return MaxDistanceToMoveTheBall; }

    bool IsMoveToLocationComplete();
 
    bool IsCanTakeBall();
    bool IsTakeBallComplete();

    void OnBallPassed();
    void OnBallTaked();

    void SetRotationPlayerOnBall();
    void MoveToTargetLeftOrRightStrafe(FVector Location);

    void RotateToTarget(FRotator Rotation, float DeltaTime);

    UFUNCTION(BlueprintCallable)
    void MoveToLocation(FVector TargetLocation);

    void MoveToMovingBall(FVector TargetLocation);



protected:
    virtual void BeginPlay() override;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
    UTBPlayerAnimationComponent* PlayerAnimationComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
    UTBBallComputeDataComponent* BallComputeDataComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
    UTBPlayerStateComponent* PlayerStateComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
    UTBBrainComponent* BrainComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
    UTBTextRenderComponent* TextRenderComponent;

    UPROPERTY(EditDefaultsOnly, Category = "Animation")
    float MaxDistanceToMoveTheBall = 3000.0f;

    UPROPERTY(EditDefaultsOnly, Category = "Animation")
    float MaxDistanceToStartTakeBall = 800.0f;

    UPROPERTY(EditDefaultsOnly, Category = "Animation")
    float MinDistanceToStartTakeBall = 400.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
    float RotationSpeed = 10.0f;

    UFUNCTION()
    void OnMoveCompleted(FAIRequestID RequestID, EPathFollowingResult::Type Result);

private:
    void MessageToPassedPlayer();
    void MoveToTargetLeftOrRightStrafeTick();
    float GetTimeMoveToLocation(FVector Location);

    bool bSetRotationPlayerOnBall = false;
    bool PlayerHaveBall           = false;

    FVector MoveToTargetNoRotVec        = FVector::Zero();
    bool bMoveToTargetLeftOrRightStrafe = false;
    bool bMoveToLocationComplete        = false;
};