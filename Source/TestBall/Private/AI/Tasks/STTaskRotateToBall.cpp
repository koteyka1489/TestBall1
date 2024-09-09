// Fill out your copyright notice in the Description page of Project Settings.

#include "AI/Tasks/STTaskRotateToBall.h"
#include "Player\TBPlayer.h"
#include "Components\TBBallComputeDataComponent.h"


USTTaskRotateToBall::USTTaskRotateToBall(const FObjectInitializer& ObjectInitializer) : UStateTreeTaskBlueprintBase(ObjectInitializer) {}

EStateTreeRunStatus USTTaskRotateToBall::Tick(FStateTreeExecutionContext& Context, const float DeltaTime)
{

    const auto Player = Cast<ATBPlayer>(GetOwnerActor(Context));
    if (Player)
    {
        FVector VecToBall = Player->GetBallComputeDataComponent()->GetVecPlayerToBall();
        VecToBall.Z       = 0.0f;

        FRotator TargetRotation = VecToBall.Rotation();

        Player->RotateToTarget(TargetRotation, DeltaTime);

        double DeltaAngles = FMath::Abs(FMath::FindDeltaAngleDegrees<double, double>(TargetRotation.Yaw, Player->GetActorRotation().Yaw));
 
        if (DeltaAngles < 5.0f)
        {
            FinishTask();
        }
    }
    Super::Tick(Context, DeltaTime);
    return RunStatus;
}