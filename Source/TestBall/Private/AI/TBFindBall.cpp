// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/TBFindBall.h"

UTBFindBall::UTBFindBall(const FObjectInitializer& ObjectInitializer) : UStateTreeTaskBlueprintBase(ObjectInitializer)
{
    UE_LOG(LogTemp, Warning, TEXT("FIND BAll Constructor"));
}

EStateTreeRunStatus UTBFindBall::EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition)
{
    UE_LOG(LogTemp, Warning, TEXT("FIND BALL Enter STATE"));
    Super::EnterState(Context, Transition);
    return RunStatus;
}

EStateTreeRunStatus UTBFindBall::Tick(FStateTreeExecutionContext& Context, const float DeltaTime)
{
    UE_LOG(LogTemp, Warning, TEXT("FIND BALL TICK"));
    Super::Tick(Context, DeltaTime);
    return RunStatus;
}
