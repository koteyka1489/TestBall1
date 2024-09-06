// Fill out your copyright notice in the Description page of Project Settings.

#include "AI/Tasks/STTaskShootBallToCage.h"
#include "AI\TBAIPlayer.h"
#include "Components\TBPlayerAnimationComponent.h"

USTTaskShootBallToCage::USTTaskShootBallToCage(const FObjectInitializer& ObjectInitializer) : UStateTreeTaskBlueprintBase(ObjectInitializer)
{
}

EStateTreeRunStatus USTTaskShootBallToCage::EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition)
{
    const auto ActorAI = Cast<ATBAIPlayer>(GetOwnerActor(Context));
    if (ActorAI)
    {
        auto PlayerAnimationComponent = Cast<UTBPlayerAnimationComponent>(ActorAI->GetPlayerAnimationComponent());
        if (PlayerAnimationComponent)
        {
            PlayerAnimationComponent->Shoot(ActorAI->GetDistanceToBall());
        }
    }
    Super::EnterState(Context, Transition);
    return RunStatus;
}

EStateTreeRunStatus USTTaskShootBallToCage::Tick(FStateTreeExecutionContext& Context, const float DeltaTime)
{
    const auto ActorAI = Cast<ATBAIPlayer>(GetOwnerActor(Context));
    if (ActorAI)
    {
        auto PlayerAnimationComponent = Cast<UTBPlayerAnimationComponent>(ActorAI->GetPlayerAnimationComponent());
        if (PlayerAnimationComponent)
        {
            if (!PlayerAnimationComponent->IsShootAnimationExecuted())
            {
                FinishTask();
            }
        }
    }

    Super::Tick(Context, DeltaTime);
    return RunStatus;
}
