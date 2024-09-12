// Fill out your copyright notice in the Description page of Project Settings.

#include "AI/Tasks/STTaskTakeBall.h"
#include "Player\TBPlayer.h"
#include "Components\TBPlayerStateComponent.h"
#include "Components\TBPlayerAnimationComponent.h"
#include "Components\TBBallComputeDataComponent.h"

USTTaskTakeBall::USTTaskTakeBall(const FObjectInitializer& ObjectInitializer) : UStateTreeTaskBlueprintBase(ObjectInitializer) {}

EStateTreeRunStatus USTTaskTakeBall::EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition)
{
    const auto Player = Cast<ATBPlayer>(GetOwnerActor(Context));
    if (Player)
    {
        // need rotate to ball
        //FVector OffsetMove =  Player->GetBallComputeDataComponent()->FindCorrectionPlayerPositionForTakeBall();
       // Player->MoveToTarget(OffsetMove);

    }


    Super::EnterState(Context, Transition);
    return RunStatus;
}

EStateTreeRunStatus USTTaskTakeBall::Tick(FStateTreeExecutionContext& Context, const float DeltaTime)
{

    const auto Player = Cast<ATBPlayer>(GetOwnerActor(Context));
    if (Player)
    {

        if (Player->IsCanTakeBall())
        {
            Player->GetPlayerAnimationComponent()->TakeBall();
        }

        if (Player->IsTakeBallComplete())
        {
            Player->GetPlayerStateComponent()->SetPlayerState(EPlayerState::PassBall);
            FinishTask();
        }
    }

    Super::Tick(Context, DeltaTime);
    return RunStatus;
}
