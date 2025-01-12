// Fill out your copyright notice in the Description page of Project Settings.

#include "Ball/Ball1.h"
#include "Components\SphereComponent.h"
#include "Components\TBStaticMeshComponent.h"
#include "GameFramework\ProjectileMovementComponent.h"
#include "Player/TBPlayer.h"
#include "Cage/Cage.h"
#include "Kismet/GameplayStatics.h"
#include "Components\TBPlayerAnimationComponent.h"
#include "Components\TBBallComputeDataComponent.h"
#include "Components\TBPlayerStateComponent.h"

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
    PrevBallLocation = GetActorLocation();
}

// Called every frame
void ABall1::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    CalcBallSpeed(DeltaTime);

    PrevBallLocation = GetActorLocation();

    FString Message2 = FString::Printf(TEXT("DELTA TIME - %f"), DeltaTime);
    GEngine->AddOnScreenDebugMessage(40, 1, FColor::Red, Message2);

    FString Message = FString::Printf(TEXT("SPEED  BALL IN FRAME - %f"), Speed);
    GEngine->AddOnScreenDebugMessage(41, 1, FColor::Red, Message);

}

void ABall1::CalcBallSpeed(float DeltaTime)
{
    if (PrevBallLocation == GetActorLocation())
    {
        Speed  = 0.0f;
    }
    else
    {
        FVector VectoPrevLoc = PrevBallLocation - GetActorLocation();
        if (VectoPrevLoc.Length() <= 0.1f)
        {
            Speed  = 0.0f;
        }
        else
        {
            Speed  = VectoPrevLoc.Length() / DeltaTime;
        }
    }
}

void ABall1::HandleOnHit(
    UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
    if (!StaticMeshComponent) return;
    auto Player = Cast<ATBPlayer>(OtherActor);
    if (!Player) return;

    bool PlayerReadyToShot = false;
    bool PlayerStopingBall = false;
    bool PlayerPassing     = false;
    bool PlayerTakingBall  = false;

    if (Player)
    {
        auto PlayerAnimationComponent = Cast<UTBPlayerAnimationComponent>(Player->GetPlayerAnimationComponent());

        if (PlayerAnimationComponent)
        {
            PlayerReadyToShot = PlayerAnimationComponent->IsShootAnimationExecuted();
            PlayerStopingBall = PlayerAnimationComponent->IsStopingBall();
            PlayerPassing     = PlayerAnimationComponent->IsPassAnimationExecuted();
            PlayerTakingBall  = PlayerAnimationComponent->IsTakeBallAnimationExecuted() ||
                               Player->GetPlayerStateComponent()->GetPlayerState() == EPlayerState::TakePassingBall;
        }
    }

    // Random Take Ball
    if (!PlayerReadyToShot && !PlayerStopingBall && !PlayerPassing && !PlayerTakingBall)
    {
        FVector LinearVelCalc  = StaticMeshComponent->GetPhysicsLinearVelocity();
        FVector AngularVelCalc = StaticMeshComponent->GetPhysicsAngularVelocityInDegrees();

        StaticMeshComponent->SetPhysicsLinearVelocity(LinearVelCalc);
        StaticMeshComponent->SetPhysicsAngularVelocityInDegrees(AngularVelCalc);
    }

    // SHOOT
    if (PlayerReadyToShot)
    {

        ShootingData ShootingData = Player->GetBallComputeDataComponent()->GetShootingData();

        StaticMeshComponent->SetPhysicsLinearVelocity(ShootingData.ShootingDirection);
        StaticMeshComponent->SetPhysicsAngularVelocityInDegrees(ShootingData.ShootingRotation);
        Player->SetPlayerHaveBall(false);
        return;
    }

    // PASS
    if (PlayerPassing)
    {
        PassingData Passing = Player->GetBallComputeDataComponent()->GetPassingData();

        FString Message = FString::Printf(TEXT("Passing Velocity - %s"), *Passing.PassDirection.ToString());
        GEngine->AddOnScreenDebugMessage(1, 3, FColor::Red, Message);

        StaticMeshComponent->SetPhysicsLinearVelocity(Passing.PassDirection);
        StaticMeshComponent->SetPhysicsAngularVelocityInDegrees(Passing.PassRotation);
        Player->OnBallPassed();

        return;
    }

    // Stop Ball
    if (PlayerStopingBall)
    {
        StaticMeshComponent->SetPhysicsLinearVelocity(FVector::Zero());
        StaticMeshComponent->SetPhysicsAngularVelocityInDegrees(FVector::Zero());

        return;
    }

    // Take Ball
    if (PlayerTakingBall)
    {
        FVector LinVel = Player->GetActorForwardVector() * 100;
        StaticMeshComponent->SetPhysicsLinearVelocity(LinVel);
        StaticMeshComponent->SetPhysicsAngularVelocityInDegrees(FVector::Zero());
        Player->OnBallTaked();

        return;
    }
}
