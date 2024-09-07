// Fill out your copyright notice in the Description page of Project Settings.

#include "AI/Tasks/STTaskTakeBall.h"
#include "AI/TBAIPlayer.h"
#include "Components\TBPlayerAnimationComponent.h"

USTTaskTakeBall::USTTaskTakeBall(const FObjectInitializer& ObjectInitializer) : UStateTreeTaskBlueprintBase(ObjectInitializer) {}

EStateTreeRunStatus USTTaskTakeBall::EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition)
{
    Super::EnterState(Context, Transition);
    return RunStatus;
}

EStateTreeRunStatus USTTaskTakeBall::Tick(FStateTreeExecutionContext& Context, const float DeltaTime)
{

    const auto ActorAI = Cast<ATBAIPlayer>(GetOwnerActor(Context));
    if (ActorAI)
    {

        if (ActorAI->IsCanTakeBall())
        {
            ActorAI->TakeBall();
        }

        if (ActorAI->IsTakeBallComplete())
        {
            ActorAI->SetStateTreeEnterCondition(EPlayerState::PassBall);
            FinishTask();
        }
    }

    Super::Tick(Context, DeltaTime);
    return RunStatus;
}
