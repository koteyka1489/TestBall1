// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TBPlayerAnimationComponent.generated.h"

class ABall1;
class ATBPlayer;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class TESTBALL_API UTBPlayerAnimationComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UTBPlayerAnimationComponent();

    UFUNCTION(BlueprintCallable, Category = "Ball")
    bool IsStopingBall() { return StopingBall; }

    UFUNCTION(BlueprintCallable, Category = "Ball")
    void SetStoppingBall(bool inState) { StopingBall = inState; }

    float GetShootTheBallDistance() { return ShootTheBallDistance; }
    float GetPassBallDistance() { return PassBallDistance; }

    bool Shoot(float VecToBallLenght);
    bool IsShootAnimationExecuted() { return ShootAnimationExecuted; }
    void PassBall(float VecToBallLenght);
    bool IsPassAnimationExecuted() { return PassAnimationExecuted; }
    void TakeBall();
    bool IsTakeBallAnimationExecuted() { return TakeBallAnimationExecuted; }

protected:
    virtual void BeginPlay() override;

    UPROPERTY(EditDefaultsOnly, Category = "Animation")
    UAnimMontage* ShotAnimMontage;

    UPROPERTY(EditDefaultsOnly, Category = "Animation")
    UAnimMontage* PassAnimMontage;

    UPROPERTY(EditDefaultsOnly, Category = "Animation")
    UAnimMontage* TakeBallAnimMontage;

    UPROPERTY(EditDefaultsOnly, Category = "Animation")
    float ShootTheBallDistance = 135.0f;

    UPROPERTY(EditDefaultsOnly, Category = "Animation")
    float PassBallDistance = 80.0f;

private:
    void PlayAnimMontage(UAnimMontage* Montage);
    void InitAnimationNotify();
    void InitShotAnimNotify();
    void InitPassAnimNotify();
    void InitTakeBallAnimNotify();

    void OnShootAnimationFinished();
    void OnPassAnimationFinished();
    void OnTakeBallAnimationFinished();

    TObjectPtr<ABall1> Ball        = nullptr;
    TObjectPtr<ATBPlayer> Player   = nullptr;
    bool ReadyToShoot              = false;
    bool ShootAnimationExecuted    = false;
    bool StopingBall               = false;
    bool PassAnimationExecuted     = false;
    bool TakeBallAnimationExecuted = false;
    bool StartTakeBallTask = false;
};
