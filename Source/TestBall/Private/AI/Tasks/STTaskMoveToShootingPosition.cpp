// Fill out your copyright notice in the Description page of Project Settings.

#include "AI/Tasks/STTaskMoveToShootingPosition.h"
#include "Player\TBPlayer.h"
#include "Components\TBPlayerAnimationComponent.h"
#include "Components\TBBallComputeDataComponent.h"

USTTaskMoveToShootingPosition::USTTaskMoveToShootingPosition(const FObjectInitializer& ObjectInitializer)
    : UStateTreeTaskBlueprintBase(ObjectInitializer)
{
}

EStateTreeRunStatus USTTaskMoveToShootingPosition::EnterState(
    FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition)
{
    const auto Player = Cast<ATBPlayer>(GetOwnerActor(Context));
    if (Player)
    {
        TargetLocation = Player->GetBallComputeDataComponent()->FindVecMoveToShootBallPosition();
        if (Player->GetBallComputeDataComponent()->IsBallMoving())
        {
            Player->MoveToMovingBall(TargetLocation);
        }
        else
        {
            Player->MoveToLocation(TargetLocation);
        }
    }

    Super::EnterState(Context, Transition);
    return RunStatus;
}

EStateTreeRunStatus USTTaskMoveToShootingPosition::Tick(FStateTreeExecutionContext& Context, const float DeltaTime)
{
    const auto Player = Cast<ATBPlayer>(GetOwnerActor(Context));
    if (Player)
    {
        FVector NewTargetLocation = Player->GetBallComputeDataComponent()->FindVecMoveToShootBallPosition();
        if ((NewTargetLocation - TargetLocation).Length() > 500.0f)
        {
            Player->MoveToLocation(NewTargetLocation);
            TargetLocation = NewTargetLocation;
        }

        bool PlayerCloseBallForShoot =
            Player->GetBallComputeDataComponent()->IsPlayerCloseBallForMotion(Player->GetPlayerAnimationComponent()->GetShootBallDistance());
        if (Player->IsMoveToLocationComplete() && PlayerCloseBallForShoot)
        {
            FinishTask();
        }
        FString Message =
            FString::Printf(TEXT("IsMoveToLocationComplete - %s"), Player->IsMoveToLocationComplete() ? TEXT("TRUE") : TEXT("FALSE"));
        GEngine->AddOnScreenDebugMessage(1, 3.0f, FColor::Emerald, Message);

         FString Message1 = FString::Printf(TEXT("PlayerCloseBallForShoot - %s"), PlayerCloseBallForShoot ? TEXT("TRUE") : TEXT("FALSE"));
        GEngine->AddOnScreenDebugMessage(2, 3.0f, FColor::Emerald, Message1);

    }

    Super::Tick(Context, DeltaTime);
    return RunStatus;
}
