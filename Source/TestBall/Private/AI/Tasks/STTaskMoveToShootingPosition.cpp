// Fill out your copyright notice in the Description page of Project Settings.

#include "AI/Tasks/STTaskMoveToShootingPosition.h"
#include "Player\TBPlayer.h"
#include "Components\TBPlayerAnimationComponent.h"
#include "Components\TBBallComputeDataComponent.h"

USTTaskMoveToShootingPosition::USTTaskMoveToShootingPosition(const FObjectInitializer& ObjectInitializer)
    : UStateTreeTaskBlueprintBase(ObjectInitializer)
{
}

EStateTreeRunStatus USTTaskMoveToShootingPosition::EnterState(
    FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition)
{
    const auto Player = Cast<ATBPlayer>(GetOwnerActor(Context));
    if (Player)
    {
        TargetLocation = Player->GetBallComputeDataComponent()->FindVecMoveToShootBallPosition();
        Player->MoveToLocation(TargetLocation);
    }

    Super::EnterState(Context, Transition);
    return RunStatus;
}

EStateTreeRunStatus USTTaskMoveToShootingPosition::Tick(FStateTreeExecutionContext& Context, const float DeltaTime)
{
    const auto Player = Cast<ATBPlayer>(GetOwnerActor(Context));
    if (Player)
    {
        FVector NewTargetLocation = Player->GetBallComputeDataComponent()->FindVecMoveToShootBallPosition();
        if ((NewTargetLocation - TargetLocation).Length() > 300.0f)
        {
            Player->MoveToLocation(NewTargetLocation);
            TargetLocation = NewTargetLocation;
        }
        if (Player->IsMoveToLocationComplete())
        {
            FinishTask();
        }
    }

    Super::Tick(Context, DeltaTime);
    return RunStatus;
}
