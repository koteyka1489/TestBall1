// Fill out your copyright notice in the Description page of Project Settings.

#include "AI/Tasks/STTaskMoveToShootingPosition.h"
#include "Player\TBPlayer.h"
#include "Components\TBPlayerAnimationComponent.h"

USTTaskMoveToShootingPosition::USTTaskMoveToShootingPosition(const FObjectInitializer& ObjectInitializer)
    : UStateTreeTaskBlueprintBase(ObjectInitializer)
{
}

EStateTreeRunStatus USTTaskMoveToShootingPosition::Tick(FStateTreeExecutionContext& Context, const float DeltaTime)
{
    const auto Player = Cast<ATBPlayer>(GetOwnerActor(Context));
    if (Player)
    {
        FVector Target = Player->FindVecMoveToShootBallPosition();
        Player->MoveToTarget(Target);

        if (Player->IsCanMakeShoot())
        {
            FinishTask();
        }
    }

    Super::Tick(Context, DeltaTime);
    return RunStatus;
}
