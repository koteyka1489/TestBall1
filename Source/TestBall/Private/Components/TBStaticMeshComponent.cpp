// Fill out your copyright notice in the Description page of Project Settings.

#include "Components/TBStaticMeshComponent.h"
#include "Ball/Ball1.h"
#include "Player/TBPlayer.h"
#include "GameFramework\ProjectileMovementComponent.h"

class ATBPlayerr;
class UProjectileMovementComponent;

UTBStaticMeshComponent::UTBStaticMeshComponent()
{
    SetSimulatePhysics(true);
}

void UTBStaticMeshComponent::BeginPlay()
{
    Super::BeginPlay();

   
}


