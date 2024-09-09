// Fill out your copyright notice in the Description page of Project Settings.

#include "AI/Tasks/STTaskTBMoveToBall.h"
#include "Player\TBPlayer.h"
#include "Components\TBBallComputeDataComponent.h"

USTTaskTBMoveToBall::USTTaskTBMoveToBall(const FObjectInitializer& ObjectInitializer) : UStateTreeTaskBlueprintBase(ObjectInitializer) {}

EStateTreeRunStatus USTTaskTBMoveToBall::Tick(FStateTreeExecutionContext& Context, const float DeltaTime)
{
    const auto Player = Cast<ATBPlayer>(GetOwnerActor(Context));
    if (Player)
    {
        Player->MoveToTarget(Player->GetBallComputeDataComponent()->GetBallLocation());
    }

    if (Player->IsMoveToBallComplete())
    {
        FinishTask();
    }

    Super::Tick(Context, DeltaTime);
    return RunStatus;
}
