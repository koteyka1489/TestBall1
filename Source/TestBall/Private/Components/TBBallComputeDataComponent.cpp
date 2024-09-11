// Fill out your copyright notice in the Description page of Project Settings.

#include "Components/TBBallComputeDataComponent.h"
#include "Ball\Ball1.h"
#include "Player/TBPlayer.h"
#include "Components\TBPlayerAnimationComponent.h"
#include "Components\TBStaticMeshComponent.h"
#include "Cage/Cage.h"
#include "Kismet\GameplayStatics.h"
#include "Components\TBBrainComponent.h"

UTBBallComputeDataComponent::UTBBallComputeDataComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
    Player                            = Cast<ATBPlayer>(GetOwner());
    Ball                              = Cast<ABall1>(UGameplayStatics::GetActorOfClass(GetWorld(), ABall1::StaticClass()));
}

void UTBBallComputeDataComponent::BeginPlay()
{
    Super::BeginPlay();

    
}

FVector UTBBallComputeDataComponent::GetBallLocation()
{
    if (Ball)
    {
        return Ball->GetActorLocation();
    }
    return FVector::Zero();
}

float UTBBallComputeDataComponent::GetDistanceToBall()
{
    if (Player)
    {
        return (GetBallLocation() - Player->GetActorLocation()).Length();
    }
    return 0.0f;
}

FVector UTBBallComputeDataComponent::FindVecMoveToShootBallPosition()
{
    if (Player)
    {
        FVector VecToBall     = GetVecPlayerToBall();
        float VecToBallLenght = VecToBall.Length();

        float ShootTheBallDistance = Player->GetPlayerAnimationComponent()->GetShootTheBallDistance();

        float GoalVecLenght = VecToBallLenght - ShootTheBallDistance + (ShootTheBallDistance / 3);

        return Player->GetActorLocation() + (VecToBall.GetSafeNormal() * GoalVecLenght);
    }
    return FVector::Zero();
}

FVector UTBBallComputeDataComponent::FindVecMoveToPassBallPosition()
{
    if (Player)
    {
        FVector VecToBall     = GetVecPlayerToBall();
        float VecToBallLenght = VecToBall.Length();

        float PassBallDistance = Player->GetPlayerAnimationComponent()->GetPassBallDistance();

        float GoalVecLenght = VecToBallLenght - PassBallDistance + (PassBallDistance / 3);

        return Player->GetActorLocation() + (VecToBall.GetSafeNormal() * GoalVecLenght);
    }
    return FVector::Zero();
}

ShootingData UTBBallComputeDataComponent::GetShootingData()
{
    ShootingData Result;

    FVector VectorAngularVelocityInDegrees = Ball->GetStaticMeshComponent()->GetPhysicsAngularVelocityInDegrees();
    Result.ShootingRotation                = VectorAngularVelocityInDegrees.GetSafeNormal();

    FVector CageLocation = Player->GetBrainComponent()->GetOpponentGoalPost()->GetActorLocation();

    FVector VectorToCage = CageLocation - Player->GetActorLocation();

    float XRand = FMath::FRandRange(-ShootingRandoms.x, ShootingRandoms.x);
    float YRand = FMath::FRandRange(-ShootingRandoms.y, ShootingRandoms.y);
    float ZRand = FMath::FRandRange(ShootingRandoms.zMin, ShootingRandoms.zMax);
    FVector RandOffset(XRand, YRand, ZRand);

    FVector VectorToGoal = VectorToCage + RandOffset;

    FVector VectorToGoalNormalize = VectorToGoal.GetSafeNormal();
    Result.ShootingDirection      = VectorToGoalNormalize * ShootingStrench;

    return Result;
}

PassingData UTBBallComputeDataComponent::GetPassingData()
{
    PassingData Result{};
    if (Player)
    {
        auto PassedPlayer    = Player->GetBrainComponent()->GetPassedPlayer();

        FString Message = FString::Printf(TEXT("TEAM N - %s"), *PassedPlayer->GetActorLocation().ToString());
        GEngine->AddOnScreenDebugMessage(6, 3, FColor::Red, Message);

        Result.PassDirection = PassedPlayer->GetActorLocation() - Player->GetActorLocation();
        Result.PassRotation  = FVector::Zero();
        return Result;
    }

    return Result;
}

void UTBBallComputeDataComponent::CheckBallLocationAndDirection()
{
    if (Ball)
    {
        CheckPlayerToBallDirection();
        CheckBallLocation();
    }
    else
    {
        checkNoEntry();
        TEXT("Ball not found");
    }
}

void UTBBallComputeDataComponent::CheckPlayerToBallDirection()
{
    VectorToBall                   = GetVecPlayerToBall();
    float DotProductBallAndePlayer = FVector::DotProduct(VectorToBall, Player->GetActorForwardVector());
    if (DotProductBallAndePlayer > 0.5)
    {
        BallIsForward = true;
    }
    else
    {
        BallIsForward = false;
    }
}

void UTBBallComputeDataComponent::CheckBallLocation()
{
    VectorToBall    = GetVecPlayerToBall();
    float VecLenght = VectorToBall.Length();

    if (VecLenght >= Player->GetPlayerAnimationComponent()->GetShootTheBallDistance() && VecLenght <= Player->GetMaxDistanceToMoveTheBall())
    {
        BallIsCloseLocation = true;
    }
    else
    {
        BallIsCloseLocation = false;
    }
}

FVector UTBBallComputeDataComponent::GetVecPlayerToBall()
{
    if (Player)
    {
        return GetBallLocation() - Player->GetActorLocation();
    }
    return FVector::Zero();
}

FVector UTBBallComputeDataComponent::FindCorrectionPlayerPositionForTakeBall()
{
    
    FVector BallVelocity = Ball->GetVelocity();
    FVector PlayerPosition = Player->GetActorLocation();
    FVector Result         = PlayerPosition;
    if (BallVelocity.Length() < 100) return Result;
   
    
    FVector PlayerRightUnitVec = Player->GetActorRightVector();
    FVector BallPostion        = Ball->GetActorLocation();
    
    FVector BallPosAddVel      = BallPostion + BallVelocity;

    FVector VectorOffset          = PlayerPosition - BallVelocity;
    FVector VectorOffsetNormalize = VectorOffset.GetSafeNormal();
    float DotProduct              = VectorOffsetNormalize.Dot(PlayerRightUnitVec);
    if (DotProduct <= -0.5f)
    {
        //Need Right Move
        Result = PlayerPosition + (PlayerRightUnitVec * VectorOffset.Length());
    }
    if (DotProduct >= 0.5f)
    {
        // Need Left Move
        Result = PlayerPosition + (PlayerRightUnitVec * -1.0f * VectorOffset.Length());
    }


    FString Message = FString::Printf(TEXT("Vector Offset - %s"), *VectorOffset.ToString());
    GEngine->AddOnScreenDebugMessage(8, 10, FColor::Cyan, Message);

    FString Message1 = FString::Printf(TEXT("Dot Product - %f"), DotProduct);
    GEngine->AddOnScreenDebugMessage(9, 10, FColor::Cyan, Message1);

    FString Message2 = FString::Printf(TEXT("Result - %s"), *Result.ToString());
    GEngine->AddOnScreenDebugMessage(10, 10, FColor::Cyan, Message2);

    return Result;
}
