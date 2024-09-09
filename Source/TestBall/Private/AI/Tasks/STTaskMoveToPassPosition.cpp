// Fill out your copyright notice in the Description page of Project Settings.

#include "AI/Tasks/STTaskMoveToPassPosition.h"
#include "Components\TBPlayerAnimationComponent.h"
#include "Player\TBPlayer.h"
#include "Components\TBBallComputeDataComponent.h"

USTTaskMoveToPassPosition::USTTaskMoveToPassPosition(const FObjectInitializer& ObjectInitializer)
    : UStateTreeTaskBlueprintBase(ObjectInitializer)
{
}

EStateTreeRunStatus USTTaskMoveToPassPosition::Tick(FStateTreeExecutionContext& Context, const float DeltaTime)
{
    const auto Player = Cast<ATBPlayer>(GetOwnerActor(Context));
    if (Player)
    {
        FVector Target = Player->GetBallComputeDataComponent()->FindVecMoveToPassBallPosition();

        Player->MoveToTarget(Target);

        if (Player->IsCanMakePass())
        {
            FinishTask();
        }
    }

    Super::Tick(Context, DeltaTime);
    return RunStatus;
}
