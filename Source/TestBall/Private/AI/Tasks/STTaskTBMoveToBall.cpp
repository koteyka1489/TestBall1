// Fill out your copyright notice in the Description page of Project Settings.

#include "AI/Tasks/STTaskTBMoveToBall.h"
#include "AI/TBAIPlayer.h"

USTTaskTBMoveToBall::USTTaskTBMoveToBall(const FObjectInitializer& ObjectInitializer) : UStateTreeTaskBlueprintBase(ObjectInitializer) {}

EStateTreeRunStatus USTTaskTBMoveToBall::Tick(FStateTreeExecutionContext& Context, const float DeltaTime)
{
    const auto ActorAI = Cast<ATBAIPlayer>(GetOwnerActor(Context));
    if (ActorAI)
    {
        ActorAI->MoveToTarget(ActorAI->GetBallLocation());
    }

    if (ActorAI->IsMoveToBallComplete())
    {
        FinishTask();
    }

    Super::Tick(Context, DeltaTime);
    return RunStatus;
}
