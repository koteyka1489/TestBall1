// Fill out your copyright notice in the Description page of Project Settings.

#include "AI/Tasks/STTaskMoveToPassPosition.h"
#include "Components\TBPlayerAnimationComponent.h"
#include "Player\TBPlayer.h"
#include "Components\TBBallComputeDataComponent.h"

USTTaskMoveToPassPosition::USTTaskMoveToPassPosition(const FObjectInitializer& ObjectInitializer)
    : UStateTreeTaskBlueprintBase(ObjectInitializer)
{
}

EStateTreeRunStatus USTTaskMoveToPassPosition::EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition)
{
    const auto Player = Cast<ATBPlayer>(GetOwnerActor(Context));
    if (Player)
    {

        if (Player->GetBallComputeDataComponent()->IsBallMovingAway())
        {
            Player->MoveToLocation(Player->GetBallComputeDataComponent()->GetBallLocation());
        }
        else
        {
            FVector TargetToPass = Player->GetBallComputeDataComponent()->FindVecMoveToPassBallPosition();
            Player->MoveToLocation(TargetToPass);
        }
    }

    Super::EnterState(Context, Transition);
    return RunStatus;
}

EStateTreeRunStatus USTTaskMoveToPassPosition::Tick(FStateTreeExecutionContext& Context, const float DeltaTime)
{
    const auto Player = Cast<ATBPlayer>(GetOwnerActor(Context));

    if (Player->IsCanMakePass())
    {
        FinishTask();
    }

    Super::Tick(Context, DeltaTime);
    return RunStatus;
}
