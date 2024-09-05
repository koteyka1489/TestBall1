// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Player/TBPlayer.h"
#include "TBAIPlayer.generated.h"

class ATBAIController;

UENUM(BlueprintType)
enum class EPlayerState : uint8
{
    MoveToBallAndControl UMETA(DisplayName = "MoveToBallAndControl"),
    MoveToBallAndShooting UMETA(DisplayName = "MoveToBallAndShooting"),
    PassBall UMETA(DisplayName = "PassBall"),
    TakePassingBall UMETA(DisplayName = "TakePassingBall")
};
/**
 *
 */
UCLASS()
class TESTBALL_API ATBAIPlayer : public ATBPlayer
{
    GENERATED_BODY()

public:
    ATBAIPlayer();

    UFUNCTION(BlueprintCallable)
    void MoveToTarget(FVector Location);

    void RotateToTarget(FRotator Rotation, float DeltaTime);

    void SetStateTreeEnterCondition(EPlayerState State_in);

protected:
    virtual void Tick(float DeltaTime) override;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
    float RotationSpeed = 5.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerState")
    EPlayerState StateTreeEnterConditions = EPlayerState::MoveToBallAndControl;

private:
    void UpdatePlayerState();
};
