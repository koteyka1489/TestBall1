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
}
