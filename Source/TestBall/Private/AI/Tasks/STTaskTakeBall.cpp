// Fill out your copyright notice in the Description page of Project Settings.

#include "AI/Tasks/STTaskTakeBall.h"
#include "AI/TBAIPlayer.h"

USTTaskTakeBall::USTTaskTakeBall(const FObjectInitializer& ObjectInitializer) : UStateTreeTaskBlueprintBase(ObjectInitializer) {}

EStateTreeRunStatus USTTaskTakeBall::EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition)
{

    const auto ActorAI = Cast<ATBAIPlayer>(GetOwnerActor(Context));


    Super::EnterState(Context, Transition);
    return RunStatus;
}

EStateTreeRunStatus USTTaskTakeBall::Tick(FStateTreeExecutionContext& Context, const float DeltaTime)
{

    const auto ActorAI = Cast<ATBAIPlayer>(GetOwnerActor(Context));
    if (ActorAI)
    {
        float DistanceToBall = ActorAI->GetDistanceToBall();

        FString Message =
            FString::Printf(TEXT("TakeBallAnimationExecuted - %s"), ActorAI->IsTakeBallAnimationExecuted() ? TEXT("True") : TEXT("False"));
        GEngine->AddOnScreenDebugMessage(3, 3, FColor::Red, Message);

        if (DistanceToBall < 800.0f && DistanceToBall > 400.0f && !ActorAI->IsTakeBallAnimationExecuted())
        {
            ActorAI->TakeBall();
        }

        if (ActorAI->IsTakeBallAnimationExecuted() && ActorAI->IsPlayerHaveBall())
        {
            FinishTask();
            ActorAI->SetStateTreeEnterCondition(EPlayerState::PassBall);
        }
    }

    Super::Tick(Context, DeltaTime);
    return RunStatus;
}
