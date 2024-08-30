// Fill out your copyright notice in the Description page of Project Settings.

#include "AI/TBAIPlayer.h"
#include "AI\TBAIController.h"
#include "Ball\Ball1.h"

ATBAIPlayer::ATBAIPlayer()
{
    AutoPossessAI     = EAutoPossessAI::PlacedInWorldOrSpawned;
    AIControllerClass = ATBAIController::StaticClass();
}

void ATBAIPlayer::BeginPlay()
{
    AiController = Cast<ATBAIController>(GetController());
}

void ATBAIPlayer::MoveToTarget(FVector Location)
{

    if (AiController)
    {
        AiController->MoveToLocation(Location);
        
    }
}

//bool ATBAIPlayer::Shoot(float VecToBallLenght)
//{
//
//    if (AiController && Ball)
//    {
//        FVector TargetVector    = Ball->GetActorLocation() - this->GetActorLocation();
//        TargetVector.Z          = 0.0f;
//        FRotator TargetRotation = TargetVector.Rotation();
//
//        AiController->SetControlRotation(TargetRotation);
//    }
//    Super::Shoot(VecToBallLenght);
//    return true;
//}
