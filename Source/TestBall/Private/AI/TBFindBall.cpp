// Fill out your copyright notice in the Description page of Project Settings.

#include "AI/TBFindBall.h"
#include "AI\TBAIPlayer.h"

UTBFindBall::UTBFindBall(const FObjectInitializer& ObjectInitializer) : UStateTreeTaskBlueprintBase(ObjectInitializer)
{
    // UE_LOG(LogTemp, Warning, TEXT("FIND BAll Constructor"));
}

EStateTreeRunStatus UTBFindBall::EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition)
{

    Super::EnterState(Context, Transition);
    return RunStatus;
}

EStateTreeRunStatus UTBFindBall::Tick(FStateTreeExecutionContext& Context, const float DeltaTime)
{
    const auto ActorAI = Cast<ATBAIPlayer>(GetOwnerActor(Context));
    if (ActorAI)
    {
        ActorAI->SetClosebleBall();
        ClosestBallLocation = ActorAI->GetClosebleBallLocation();
        //UE_LOG(LogTemp, Warning, TEXT("CloseBall %s"), *CloseBall.ToString());
    }

    Super::Tick(Context, DeltaTime);
    return RunStatus;
}
