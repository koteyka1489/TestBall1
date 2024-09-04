// Fill out your copyright notice in the Description page of Project Settings.

#include "AI/Tasks/STTaskShootBallToCage.h"
#include "AI\TBAIPlayer.h"

USTTaskShootBallToCage::USTTaskShootBallToCage(const FObjectInitializer& ObjectInitializer) : UStateTreeTaskBlueprintBase(ObjectInitializer)
{
}

EStateTreeRunStatus USTTaskShootBallToCage::EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition)
{
    const auto ActorAI = Cast<ATBAIPlayer>(GetOwnerActor(Context));
    if (ActorAI)
    {
        ActorAI->Shoot(ActorAI->GetDistanceToBall());
    }
    Super::EnterState(Context, Transition);
    return RunStatus;
}

EStateTreeRunStatus USTTaskShootBallToCage::Tick(FStateTreeExecutionContext& Context, const float DeltaTime)
{
    const auto ActorAI = Cast<ATBAIPlayer>(GetOwnerActor(Context));
    if (!ActorAI->IsShootAnimationExecuted())
    {
        FinishTask();
    }

    Super::Tick(Context, DeltaTime);
    return RunStatus;
}
