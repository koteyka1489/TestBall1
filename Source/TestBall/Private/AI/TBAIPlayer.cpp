// Fill out your copyright notice in the Description page of Project Settings.

#include "AI/TBAIPlayer.h"
#include "AI\TBAIController.h"

ATBAIPlayer::ATBAIPlayer()
{
    AutoPossessAI     = EAutoPossessAI::PlacedInWorldOrSpawned;
    AIControllerClass = ATBAIController::StaticClass();
}

void ATBAIPlayer::MoveToTarget(FVector Location)
{
    AAIController* AiController = Cast<AAIController>(GetController());
    if (AiController)
    {

        AiController->MoveToLocation(Location);
    }
}
