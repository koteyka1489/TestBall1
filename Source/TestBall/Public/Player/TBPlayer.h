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
    FVector GetClosebleBallLocation();

    UFUNCTION(BlueprintCallable, Category = "Ball")
    bool GetPlayerHitedBall();

    UFUNCTION(BlueprintCallable, Category = "Ball")
    bool SetPlayerHitedBall(bool in);



protected:
    virtual void BeginPlay() override;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
    USpringArmComponent* SpringArmComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
    UCameraComponent* CameraComponent;

    UPROPERTY(EditDefaultsOnly, Category = "Animation")
    UAnimMontage* ShotAnimMontage;

    UPROPERTY(EditDefaultsOnly, Category = "Animation")
    float ShotTheBallDistance = 115.0f;

    UPROPERTY(EditDefaultsOnly, Category = "Animation")
    float MaxDistanceToMoveTheBall = 3000.0f;

    UPROPERTY(EditDefaultsOnly, Category = "Animation")
    bool LockCameraOnShoot = false;

    UPROPERTY()
    TArray<AActor*> Balls;

    UPROPERTY()
    TObjectPtr<ABall1> ClosebleBall = nullptr;

public:
    virtual void Tick(float DeltaTime) override;

    // Called to bind functionality to input
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
    void MoveForward(float Amount);
    void MoveRight(float Amount);
    void Shoot();
    void CheckBallLocationandDirection(ABall1* Ball);
    void CheckPlayerToBallDirection(ABall1* Ball);
    void CheckBallLocation(ABall1* Ball);
    void MoveToBall();
    void InitAnimationNotify();
    void OnShootAnimationFinished();
    void SetClosebleBall();
    void OnBallHit();
    void LockCamera();

    bool IsMovingToBall      = false;
    bool BallIsForward       = false;
    bool BallIsCloseLocation = false;
    bool ReadyToShoot        = false;
    bool IsPlayerHitedBall     = false;
    FVector VectorToBall     = FVector::ZeroVector;
};
