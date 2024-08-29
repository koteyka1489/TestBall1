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
    float GetDistanceToCloseballBall();

    FVector FindVecMoveToShootBallPosition();

    float GetShootTheBallDistance() { return ShootTheBallDistance; }

    void SetClosebleBall();
    bool Shoot(float VecToBallLenght);
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
    void CheckBallLocationandDirection(ABall1* Ball);
    void CheckPlayerToBallDirection(ABall1* Ball);
    void CheckBallLocation(ABall1* Ball);
    
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
