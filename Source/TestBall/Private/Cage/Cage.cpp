// Fill out your copyright notice in the Description page of Project Settings.


#include "Cage/Cage.h"
#include "Components\TBStaticMeshComponent.h"


// Sets default values
ACage::ACage()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	StaticMeshComponent = CreateDefaultSubobject<UTBStaticMeshComponent>("StaticMeshComponent");
    SetRootComponent(StaticMeshComponent);

}

// Called when the game starts or when spawned
void ACage::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACage::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

