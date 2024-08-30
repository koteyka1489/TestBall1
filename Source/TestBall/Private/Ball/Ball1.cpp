// Fill out your copyright notice in the Description page of Project Settings.

#include "Ball/Ball1.h"
#include "Components\SphereComponent.h"
#include "Components\TBStaticMeshComponent.h"
#include "GameFramework\ProjectileMovementComponent.h"
#include "Player/TBPlayer.h"
#include "Cage/Cage.h"
#include "Kismet/GameplayStatics.h"

struct ShootingData;

// Sets default values
ABall1::ABall1()
{
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    StaticMeshComponent = CreateDefaultSubobject<UTBStaticMeshComponent>("StaticMeshComponent");
    SetRootComponent(StaticMeshComponent);

    StaticMeshComponent->OnComponentHit.AddDynamic(this, &ABall1::HandleOnHit);
}

// Called when the game starts or when spawned
void ABall1::BeginPlay()
{
    Super::BeginPlay();
}

// Called every frame
void ABall1::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void ABall1::HandleOnHit(
    UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{

    FVector LinearVelocityCalc = StaticMeshComponent->GetPhysicsLinearVelocity();
    FVector AngularVelocityCalc = StaticMeshComponent->GetPhysicsAngularVelocityInDegrees();

    auto Player = Cast<ATBPlayer>(OtherActor);
    if (Player && Player->IsRedyToShoot())
    {
        OnBallHit.Broadcast();

        ShootingData ShootingData = Player->GetShootingData();

        
        FString Message  = FString::Printf(TEXT("ShootingDirection -  %s"), *ShootingData.ShootingDirection.ToString());
        FString Message1 = FString::Printf(TEXT("ShootingRotation -  %s"), *ShootingData.ShootingRotation.ToString());
      

        StaticMeshComponent->SetPhysicsLinearVelocity(ShootingData.ShootingDirection);
        StaticMeshComponent->SetPhysicsAngularVelocityInDegrees(ShootingData.ShootingRotation);
    }
    if (Player && !Player->IsRedyToShoot())
    {
        StaticMeshComponent->SetPhysicsLinearVelocity(LinearVelocityCalc);
        StaticMeshComponent->SetPhysicsAngularVelocityInDegrees(AngularVelocityCalc);
    }
}
