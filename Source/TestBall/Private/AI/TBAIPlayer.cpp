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

bool ATBAIPlayer::Shoot(float VecToBallLenght)
{

    return Super::Shoot(VecToBallLenght);
}






