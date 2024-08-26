// Fill out your copyright notice in the Description page of Project Settings.

#include "AI/Tasks/STTaskTBMoveToBall.h"
#include "AI\TBAIPlayer.h"

USTTaskTBMoveToBall::USTTaskTBMoveToBall(const FObjectInitializer& ObjectInitializer) : UStateTreeTaskBlueprintBase(ObjectInitializer) {}

EStateTreeRunStatus USTTaskTBMoveToBall::EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition)
{
    UE_LOG(LogTemp, Warning, TEXT("MOVE TO BALL Enter STATE"));

    return RunStatus;
}

EStateTreeRunStatus USTTaskTBMoveToBall::Tick(FStateTreeExecutionContext& Context, const float DeltaTime)
{
    UE_LOG(LogTemp, Warning, TEXT("Move To BALL Tick STATE"));

    const auto ActorAI = Cast<ATBAIPlayer>(GetOwnerActor(Context));
    if (ActorAI)
    {
        ActorAI->SetClosebleBall();

        ActorAI->MoveToTarget(ActorAI->GetClosebleBallLocation());
    }

    if (ActorAI->GetDistanceToCloseballBall() < 100.f)
    {
        FinishTask();
        //return EStateTreeRunStatus::Succeeded;
    }

    return RunStatus;
}
