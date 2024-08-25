// Fill out your copyright notice in the Description page of Project Settings.

#include "Ball/Ball.h"
#include "Components\StaticMeshComponent.h"


ABall::ABall()
{
    PrimaryActorTick.bCanEverTick = true;

    SceneComponent      = CreateDefaultSubobject<USceneComponent>("SceneComponent");
    SetRootComponent(SceneComponent);
    StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("StaticMeshComponent");
    StaticMeshComponent->SetupAttachment(GetRootComponent());
}


void ABall::BeginPlay()
{
    Super::BeginPlay();
}

// Called every frame
void ABall::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}
