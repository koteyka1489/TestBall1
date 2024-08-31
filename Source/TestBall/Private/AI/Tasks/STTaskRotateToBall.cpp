// Fill out your copyright notice in the Description page of Project Settings.

#include "AI/Tasks/STTaskRotateToBall.h"
#include "AI/TBAIPlayer.h"

USTTaskRotateToBall::USTTaskRotateToBall(const FObjectInitializer& ObjectInitializer) : UStateTreeTaskBlueprintBase(ObjectInitializer) {}

EStateTreeRunStatus USTTaskRotateToBall::Tick(FStateTreeExecutionContext& Context, const float DeltaTime)
{

    const auto ActorAI = Cast<ATBAIPlayer>(GetOwnerActor(Context));
    if (ActorAI)
    {
        FVector VecToBall = ActorAI->GetBallLocation() - ActorAI->GetActorLocation();
        VecToBall.Z       = 0.0f;

        FRotator TargetRotation = VecToBall.Rotation();

        ActorAI->RotateToTarget(TargetRotation, DeltaTime);

        double DeltaAngles = FMath::Abs(FMath::FindDeltaAngleDegrees<double, double>(TargetRotation.Yaw, ActorAI->GetActorRotation().Yaw));
        UE_LOG(LogTemp, Display, TEXT("Delta %f"), DeltaAngles);
        if (DeltaAngles < 5.0f)
        {
            FinishTask();
        }
    }
    Super::Tick(Context, DeltaTime);
    return RunStatus;
}