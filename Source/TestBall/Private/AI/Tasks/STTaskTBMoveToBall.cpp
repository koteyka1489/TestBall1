// Fill out your copyright notice in the Description page of Project Settings.

#include "AI/Tasks/STTaskTBMoveToBall.h"
#include "AI\TBAIPlayer.h"

USTTaskTBMoveToBall::USTTaskTBMoveToBall(const FObjectInitializer& ObjectInitializer) : UStateTreeTaskBlueprintBase(ObjectInitializer) {}

EStateTreeRunStatus USTTaskTBMoveToBall::Tick(FStateTreeExecutionContext& Context, const float DeltaTime)
{
    UE_LOG(LogTemp, Warning, TEXT("Move To BALL Tick STATE"));

    const auto ActorAI = Cast<ATBAIPlayer>(GetOwnerActor(Context));
    if (ActorAI)
    {
        
        FVector Target = ActorAI->GetActorLocation() + ActorAI->FindVecMoveToShootBallPosition();
        ActorAI->MoveToTarget(Target);

    }



    if (ActorAI->GetDistanceToBall() < ActorAI->GetShootTheBallDistance() + 50.0)
    {
        UE_LOG(LogTemp, Display, TEXT("Finish Task %f"), ActorAI->GetShootTheBallDistance() + 50.0);
        FinishTask();
    }

    Super::Tick(Context, DeltaTime);
    return RunStatus;
}