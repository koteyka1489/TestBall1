// Fill out your copyright notice in the Description page of Project Settings.

#include "AI/Tasks/STTaskRandomRunning.h"
#include "Player/TBPlayer.h"


USTTaskRandomRunning::USTTaskRandomRunning(const FObjectInitializer& ObjectInitializer) : UStateTreeTaskBlueprintBase(ObjectInitializer) {}

EStateTreeRunStatus USTTaskRandomRunning::EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition)
{
    const auto Player = Cast<ATBPlayer>(GetOwnerActor(Context));
    float XRand = FMath::FRandRange(0.0f, 2000.0f);
    float YRand = FMath::FRandRange(0.0f, 2000.0f);
    float ZOld  = Player->GetActorLocation().Z;
    NewLocation       = FVector{XRand, YRand, ZOld};

    Super::EnterState(Context, Transition);
    return RunStatus;
}

EStateTreeRunStatus USTTaskRandomRunning::Tick(FStateTreeExecutionContext& Context, const float DeltaTime)
{
 
    const auto Player = Cast<ATBPlayer>(GetOwnerActor(Context));
    if (Player)
    {
        Player->MoveToLocation(NewLocation);
    }

    if (Player->GetActorLocation().Equals(NewLocation, 20.0f))
    {
        FinishTask();
    }


    Super::Tick(Context, DeltaTime);
    return RunStatus;
}
