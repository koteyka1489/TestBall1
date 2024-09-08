// Fill out your copyright notice in the Description page of Project Settings.

#include "AI/Tasks/STTaskTakeBall.h"
#include "Player\TBPlayer.h"
#include "Components\TBPlayerAnimationComponent.h"

USTTaskTakeBall::USTTaskTakeBall(const FObjectInitializer& ObjectInitializer) : UStateTreeTaskBlueprintBase(ObjectInitializer) {}

EStateTreeRunStatus USTTaskTakeBall::EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition)
{
    Super::EnterState(Context, Transition);
    return RunStatus;
}

EStateTreeRunStatus USTTaskTakeBall::Tick(FStateTreeExecutionContext& Context, const float DeltaTime)
{

    const auto Player = Cast<ATBPlayer>(GetOwnerActor(Context));
    if (Player)
    {

        if (Player->IsCanTakeBall())
        {
            Player->TakeBall();
        }

        if (Player->IsTakeBallComplete())
        {
            Player->SetStateTreeEnterCondition(EPlayerState::PassBall);
            FinishTask();
        }
    }

    Super::Tick(Context, DeltaTime);
    return RunStatus;
}
