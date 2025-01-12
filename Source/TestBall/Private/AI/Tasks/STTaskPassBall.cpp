// Fill out your copyright notice in the Description page of Project Settings.

#include "AI/Tasks/STTaskPassBall.h"
#include "Player\TBPlayer.h"
#include "Components\TBPlayerAnimationComponent.h"
#include "Components\TBBrainComponent.h"

USTTaskPassBall::USTTaskPassBall(const FObjectInitializer& ObjectInitializer) : UStateTreeTaskBlueprintBase(ObjectInitializer) {}

EStateTreeRunStatus USTTaskPassBall::EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition)
{
    const auto Player = Cast<ATBPlayer>(GetOwnerActor(Context));
    if (Player)
    {
        Player->GetBrainComponent()->SelectPlayerToPass();
        Player->GetPlayerAnimationComponent()->PassBall();
    }

    Super::EnterState(Context, Transition);
    return RunStatus;
}

EStateTreeRunStatus USTTaskPassBall::Tick(FStateTreeExecutionContext& Context, const float DeltaTime)
{
    const auto Player = Cast<ATBPlayer>(GetOwnerActor(Context));
    if (Player)
    {
        if (!Player->GetPlayerAnimationComponent()->IsPassAnimationExecuted())
        {
            FinishTask();
        }
    }

    Super::Tick(Context, DeltaTime);
    return RunStatus;
}


