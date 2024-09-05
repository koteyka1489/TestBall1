// Fill out your copyright notice in the Description page of Project Settings.

#include "AI/TBAIPlayer.h"
#include "AI\TBAIController.h"
#include "Ball\Ball1.h"

ATBAIPlayer::ATBAIPlayer()
{
    AutoPossessAI     = EAutoPossessAI::PlacedInWorldOrSpawned;
    AIControllerClass = ATBAIController::StaticClass();
}

void ATBAIPlayer::MoveToTarget(FVector Location)
{
    ATBAIController* AiControllerComp = Cast<ATBAIController>(GetController());
    if (AiControllerComp)
    {
        AiControllerComp->MoveToLocation(Location);
    }
}

void ATBAIPlayer::RotateToTarget(FRotator Rotation, float DeltaTime)
{
    FRotator SmoothRotation = FMath::RInterpTo(this->GetActorRotation(), Rotation, DeltaTime, RotationSpeed);
    SetActorRotation(SmoothRotation);
}

void ATBAIPlayer::SetStateTreeEnterCondition(EPlayerState State_in) 
{
    StateTreeEnterConditions = State_in;
}

void ATBAIPlayer::Tick(float DeltaTime) 
{
    Super::Tick(DeltaTime);

    UpdatePlayerState();
}

void ATBAIPlayer::UpdatePlayerState() 
{
    if (IsPlayerHaveBall())
    {
        StateTreeEnterConditions = EPlayerState::PassBall;
    }
    if (!IsPlayerHaveBall())
    {
        StateTreeEnterConditions = EPlayerState::TakePassingBall;
    }
}
