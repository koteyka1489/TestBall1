// Fill out your copyright notice in the Description page of Project Settings.

#include "AI/Tasks/STTaskTakeBall.h"
#include "AI/TBAIPlayer.h"

USTTaskTakeBall::USTTaskTakeBall(const FObjectInitializer& ObjectInitializer) : UStateTreeTaskBlueprintBase(ObjectInitializer) {}

EStateTreeRunStatus USTTaskTakeBall::Tick(FStateTreeExecutionContext& Context, const float DeltaTime)
{

    const auto ActorAI = Cast<ATBAIPlayer>(GetOwnerActor(Context));
    if (ActorAI)
    {
        float DistanceToBall = ActorAI->GetDistanceToBall();

        if (DistanceToBall < 800.0f && !ActorAI->IsTakeBallAnimationExecuted())
        {
            ActorAI->TakeBall();
        }

        if (ActorAI->IsTakeBallAnimationExecuted())
        {
            FinishTask();
            ActorAI->SetStateTreeEnterCondition(EPlayerState::PassBall);
        }
    }

    Super::Tick(Context, DeltaTime);
    return RunStatus;
}
