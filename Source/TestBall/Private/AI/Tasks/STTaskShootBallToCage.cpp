// Fill out your copyright notice in the Description page of Project Settings.

#include "AI/Tasks/STTaskShootBallToCage.h"
#include "Player\TBPlayer.h"
#include "Components\TBPlayerAnimationComponent.h"

USTTaskShootBallToCage::USTTaskShootBallToCage(const FObjectInitializer& ObjectInitializer) : UStateTreeTaskBlueprintBase(ObjectInitializer)
{
}

EStateTreeRunStatus USTTaskShootBallToCage::EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition)
{
    const auto Player = Cast<ATBPlayer>(GetOwnerActor(Context));
    if (Player)
    {
        Player->ShootBall();
    }
    Super::EnterState(Context, Transition);
    return RunStatus;
}

EStateTreeRunStatus USTTaskShootBallToCage::Tick(FStateTreeExecutionContext& Context, const float DeltaTime)
{
    const auto Player = Cast<ATBPlayer>(GetOwnerActor(Context));
    if (Player)
    {
        if (!Player->IsShootAnimationExecuted())
        {
            FinishTask();
        }
    }

    Super::Tick(Context, DeltaTime);
    return RunStatus;
}
