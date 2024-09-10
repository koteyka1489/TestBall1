// Fill out your copyright notice in the Description page of Project Settings.

#include "Components/TBBrainComponent.h"
#include "Cage/Cage.h"
#include "Player/TBPlayer.h"

UTBBrainComponent::UTBBrainComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void UTBBrainComponent::BeginPlay()
{
    Super::BeginPlay();
    PassedPlayer = Team[0];
}

void UTBBrainComponent::SelectPlayerToPass()
{
    FString Message = FString::Printf(TEXT("TEAM N - %i"), Team.Num());
    GEngine->AddOnScreenDebugMessage(5, 3, FColor::Red, Message);

    if (Team.Num() == 0) return;

    if (Team.Num() == 1)
    {
        PassedPlayer = Team[0];
    }
    else
    {
        int32 RandIndex = FMath::RandRange(0, Team.Num() - 1);
        PassedPlayer    = Team[RandIndex];
    }
}
