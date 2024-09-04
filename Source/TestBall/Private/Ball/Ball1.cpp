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

FVector ABall1::GetBallPhysicVelocity()
{
    if (!StaticMeshComponent) return FVector::Zero();

    return StaticMeshComponent->GetPhysicsLinearVelocity();
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
    if (!StaticMeshComponent) return;

    FVector LinearVelCalc  = StaticMeshComponent->GetPhysicsLinearVelocity();
    FVector AngularVelCalc = StaticMeshComponent->GetPhysicsAngularVelocityInDegrees();

    auto Player = Cast<ATBPlayer>(OtherActor);

    // Random Take Ball
    if (Player && !Player->IsRedyToShoot() && !Player->IsStopingBall() && !Player->IsPassAnimationExecuted() &&
        !Player->IsTakeBallAnimationExecuted())
    {
        StaticMeshComponent->SetPhysicsLinearVelocity(LinearVelCalc);
        StaticMeshComponent->SetPhysicsAngularVelocityInDegrees(AngularVelCalc);
    }

    // SHOOT
    if (Player && Player->IsRedyToShoot()) 
    {
        OnBallHit.Broadcast();

        ShootingData ShootingData = Player->GetShootingData();

        StaticMeshComponent->SetPhysicsLinearVelocity(ShootingData.ShootingDirection);
        StaticMeshComponent->SetPhysicsAngularVelocityInDegrees(ShootingData.ShootingRotation);
        return;
    }

    // PASS
    if (Player && Player->IsPassAnimationExecuted()) 
    {
        PassingData Passing = Player->GetPassingData();

        FString Message = FString::Printf(TEXT("Passing Velocity - %s"), *Passing.PassDirection.ToString());
        GEngine->AddOnScreenDebugMessage(1, 3, FColor::Red, Message );
        

        StaticMeshComponent->SetPhysicsLinearVelocity(Passing.PassDirection);
        StaticMeshComponent->SetPhysicsAngularVelocityInDegrees(Passing.PassRotation);
        return;
    }

   

    // Stop Ball 
    if (Player && Player->IsStopingBall())
    {
        StaticMeshComponent->SetPhysicsLinearVelocity(FVector::Zero());
        StaticMeshComponent->SetPhysicsAngularVelocityInDegrees(FVector::Zero());
        return;
    }

    //Take Ball
    if (Player && Player->IsTakeBallAnimationExecuted())
    {
        FVector LinVel = Player->GetActorForwardVector() * 100;
        StaticMeshComponent->SetPhysicsLinearVelocity(LinVel);
        StaticMeshComponent->SetPhysicsAngularVelocityInDegrees(FVector::Zero());
        return;
    }
}
