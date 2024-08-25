// Fill out your copyright notice in the Description page of Project Settings.


#include "Ball/BallActor3.h"
#include "Components\SphereComponent.h"
#include "Components\TBStaticMeshComponent.h"

// Sets default values
ABallActor3::ABallActor3()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SphereComponent = CreateDefaultSubobject<USphereComponent>("USphereComponent");
    SetRootComponent(SphereComponent);

	StaticMeshComponent = CreateDefaultSubobject<UTBStaticMeshComponent>("StaticMeshComponent");
    StaticMeshComponent->SetupAttachment(SphereComponent);



}

// Called when the game starts or when spawned
void ABallActor3::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABallActor3::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

