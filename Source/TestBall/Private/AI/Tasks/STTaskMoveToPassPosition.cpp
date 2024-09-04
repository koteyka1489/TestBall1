// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Tasks/STTaskMoveToPassPosition.h"
#include "AI/TBAIPlayer.h"

USTTaskMoveToPassPosition::USTTaskMoveToPassPosition(const FObjectInitializer& ObjectInitializer)
    : UStateTreeTaskBlueprintBase(ObjectInitializer)
{
}

EStateTreeRunStatus USTTaskMoveToPassPosition::Tick(FStateTreeExecutionContext& Context, const float DeltaTime)
{
    const auto ActorAI = Cast<ATBAIPlayer>(GetOwnerActor(Context));
    if (ActorAI)
    {
        FVector Target = ActorAI->GetActorLocation() + ActorAI->FindVecMoveToPassBallPosition();
        ActorAI->MoveToTarget(Target);
    }

    if (ActorAI->GetDistanceToBall() < ActorAI->GetPassBallDistance() + 50.0)
    {
        FinishTask();
    }

    Super::Tick(Context, DeltaTime);
    return RunStatus;
}
