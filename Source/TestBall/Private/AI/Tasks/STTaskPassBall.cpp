// Fill out your copyright notice in the Description page of Project Settings.

#include "AI/Tasks/STTaskPassBall.h"
#include "AI\TBAIPlayer.h"
#include "Components\TBPlayerAnimationComponent.h"

USTTaskPassBall::USTTaskPassBall(const FObjectInitializer& ObjectInitializer) : UStateTreeTaskBlueprintBase(ObjectInitializer) {}

EStateTreeRunStatus USTTaskPassBall::EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition)
{
    const auto ActorAI = Cast<ATBAIPlayer>(GetOwnerActor(Context));
    if (ActorAI)
    {
        ActorAI->PassBall();
    }

    Super::EnterState(Context, Transition);
    return RunStatus;
}

EStateTreeRunStatus USTTaskPassBall::Tick(FStateTreeExecutionContext& Context, const float DeltaTime)
{
    const auto ActorAI = Cast<ATBAIPlayer>(GetOwnerActor(Context));
    if (ActorAI)
    {
        if (!ActorAI->IsPassAnimationExecuted())
        {
            FinishTask();
        }
    }

    Super::Tick(Context, DeltaTime);
    return RunStatus;
}
