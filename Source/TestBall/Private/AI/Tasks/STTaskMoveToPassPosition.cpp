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
        TargetLocation = Player->GetBallComputeDataComponent()->FindVecMoveToPassBallPosition();
        if (Player->GetBallComputeDataComponent()->IsBallMoving())
        {
            Player->MoveToMovingBall(TargetLocation);
        }
        else
        {
            Player->MoveToLocation(TargetLocation);
        }
    }

    Super::EnterState(Context, Transition);
    return RunStatus;
}

EStateTreeRunStatus USTTaskMoveToPassPosition::Tick(FStateTreeExecutionContext& Context, const float DeltaTime)
{
    const auto Player = Cast<ATBPlayer>(GetOwnerActor(Context));
    if (Player)
    {
        FVector NewTargetLocation = Player->GetBallComputeDataComponent()->FindVecMoveToPassBallPosition();
        if ((NewTargetLocation - TargetLocation).Length() > 500.0f)
        {
            TargetLocation = NewTargetLocation;
            Player->MoveToMovingBall(TargetLocation);
            
        }
        if (Player->IsMoveToLocationComplete())
        {
            FinishTask();
        }
    }

    

    Super::Tick(Context, DeltaTime);
    return RunStatus;
}
