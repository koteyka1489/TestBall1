// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TBPlayerStateComponent.generated.h"

UENUM(BlueprintType)
enum class EPlayerState : uint8
{
    MoveToBallAndControl UMETA(DisplayName = "Move To Ball And Control"),
    MoveToBallAndShooting UMETA(DisplayName = "Move To Ball And Shooting"),
    PassBall UMETA(DisplayName = "Pass Ball"),
    TakePassingBall UMETA(DisplayName = "Take Passing Ball"),
    RandomRunning UMETA(DisplayName = "Random Running")
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
