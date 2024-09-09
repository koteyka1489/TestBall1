// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TBPlayerStateComponent.generated.h"

UENUM(BlueprintType)
enum class EPlayerState : uint8
{
    MoveToBallAndControl UMETA(DisplayName = "MoveToBallAndControl"),
    MoveToBallAndShooting UMETA(DisplayName = "MoveToBallAndShooting"),
    PassBall UMETA(DisplayName = "PassBall"),
    TakePassingBall UMETA(DisplayName = "TakePassingBall")
};

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class TESTBALL_API UTBPlayerStateComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UTBPlayerStateComponent();

    void SetPlayerState(EPlayerState State_in) { PlayerState = State_in; };

    EPlayerState GetPlayerState() { return PlayerState; }

protected:
    virtual void BeginPlay() override;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerState")
    EPlayerState PlayerState = EPlayerState::MoveToBallAndControl;
};
