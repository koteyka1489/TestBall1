// Fill out your copyright notice in the Description page of Project Settings.

#include "AI/Tasks/STTaskMoveToShootingPosition.h"
#include "AI/TBAIPlayer.h"
#include "Components\TBPlayerAnimationComponent.h"

USTTaskMoveToShootingPosition::USTTaskMoveToShootingPosition(const FObjectInitializer& ObjectInitializer)
    : UStateTreeTaskBlueprintBase(ObjectInitializer)
{
}

EStateTreeRunStatus USTTaskMoveToShootingPosition::Tick(FStateTreeExecutionContext& Context, const float DeltaTime)
{
    const auto ActorAI = Cast<ATBAIPlayer>(GetOwnerActor(Context));
    if (ActorAI)
    {
        FVector Target = ActorAI->FindVecMoveToShootBallPosition();
        ActorAI->MoveToTarget(Target);

        if (ActorAI->IsCanMakeShoot())
        {
            FinishTask();
        }
    }

    Super::Tick(Context, DeltaTime);
    return RunStatus;
}
