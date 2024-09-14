// Fill out your copyright notice in the Description page of Project Settings.

#include "Components/TBBallComputeDataComponent.h"
#include "Ball\Ball1.h"
#include "Player/TBPlayer.h"
#include "Components\TBPlayerAnimationComponent.h"
#include "Components\TBStaticMeshComponent.h"
#include "Cage/Cage.h"
#include "Kismet\GameplayStatics.h"
#include "Components\TBBrainComponent.h"
#include "Kismet/KismetSystemLibrary.h"

UTBBallComputeDataComponent::UTBBallComputeDataComponent()
{
    PrimaryComponentTick.bCanEverTick = true;
    Player                            = Cast<ATBPlayer>(GetOwner());
    Ball                              = Cast<ABall1>(UGameplayStatics::GetActorOfClass(GetWorld(), ABall1::StaticClass()));
}

void UTBBallComputeDataComponent::BeginPlay()
{
    Super::BeginPlay();
}

void UTBBallComputeDataComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) 
{
    CalcBallSpeed(DeltaTime); 
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

FVector UTBBallComputeDataComponent::GetBallLocation()
{
    if (Ball)
    {
        return Ball->GetActorLocation();
    }
    return FVector::Zero();
}

FVector UTBBallComputeDataComponent::GetBallVelocity()
{
    if (Ball)
    {
        return Ball->GetVelocity();
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
        auto PassedPlayer = Player->GetBrainComponent()->GetPassedPlayer();

        float XRandPass = FMath::FRandRange(-PassingRandoms.x, PassingRandoms.x);
        float YRandPass = FMath::FRandRange(-PassingRandoms.y, PassingRandoms.y);
        FVector RandPassOffset(XRandPass, YRandPass, 0.0f);
        FVector ResultPassFirection = (PassedPlayer->GetActorLocation() - Player->GetActorLocation()) + RandPassOffset;

        Result.PassDirection = ResultPassFirection;
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

void UTBBallComputeDataComponent::CalcBallSpeed(float DeltaTime)
{

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

    FVector BallVelocity   = Ball->GetVelocity();
    FVector PlayerPosition = Player->GetActorLocation();
    if (BallVelocity.Length() < 100) return PlayerPosition;

    FVector PlayerRightUnitVec = Player->GetActorRightVector();
    FVector PlayerRightVec     = PlayerPosition + (PlayerRightUnitVec * MaxTackebleLenght);
    FVector PlayerLeftVec      = PlayerPosition + (-PlayerRightUnitVec * MaxTackebleLenght);

    FVector BallPostion   = Ball->GetActorLocation();
    FVector BallPosAddVel = BallPostion + BallVelocity * 2.0f;

    FVector RightIntersection = FVector::Zero();
    FVector LeftIntersection  = FVector::Zero();

    UKismetSystemLibrary::DrawDebugArrow(GetWorld(), BallPostion, BallPosAddVel, 1.0f, FColor::Red, 1.0f, 3.0f);
    UKismetSystemLibrary::DrawDebugArrow(GetWorld(), PlayerPosition, PlayerRightVec, 1.0f, FColor::Blue, 1.0f, 3.0f);
    UKismetSystemLibrary::DrawDebugArrow(GetWorld(), PlayerPosition, PlayerLeftVec, 1.0f, FColor::Magenta, 1.0f, 3.0f);

    if (FMath::SegmentIntersection2D(BallPostion, BallPosAddVel, PlayerPosition, PlayerRightVec, RightIntersection))
    {
        FString Message = FString::Printf(TEXT("Right Intersection - %s"), *RightIntersection.ToString());
        GEngine->AddOnScreenDebugMessage(10, 1, FColor::Cyan, Message);
        FVector Offset = RightIntersection - PlayerPosition;
        FVector Result = PlayerPosition + Offset;
        return Result;
    }
    if (FMath::SegmentIntersection2D(BallPostion, BallPosAddVel, PlayerPosition, PlayerLeftVec, LeftIntersection))
    {
        FString Message = FString::Printf(TEXT("Left Intersection - %s"), *LeftIntersection.ToString());
        GEngine->AddOnScreenDebugMessage(11, 1, FColor::Cyan, Message);
        FVector Offset = LeftIntersection - PlayerPosition;
        FVector Result = PlayerPosition + Offset;
        return Result;
    }

    return PlayerPosition;
}

bool UTBBallComputeDataComponent::IsBallMovingAway()
{
    if (!Ball) return false;

    FVector BallVelocity             = GetBallVelocity();
    FVector BallVelocityNormalize    = BallVelocity.GetSafeNormal();
    float DotPlayerForwardAndBallVel = BallVelocityNormalize.Dot(Player->GetActorForwardVector());
    if (DotPlayerForwardAndBallVel > 0.1f && BallVelocity.Length() > BallVelocityMaxLenght)
    {
        GEngine->AddOnScreenDebugMessage(12, 1, FColor::Cyan, TEXT("IsBallMovingAway TRUE"));
        return true;
    }
    return false;
}

FVector UTBBallComputeDataComponent::GetBallLocationInTime(float Time)
{
    FVector BallLocation = GetBallLocation();
    FVector BallVelocity = GetBallVelocity();
    FVector VecBallToBallDest = (BallLocation + BallVelocity) - BallLocation;
    float TimeToBallDest      = 0.0f;

    if (BallVelocity.Length() != 0.0f)
    {
        TimeToBallDest = VecBallToBallDest.Length() / BallVelocity.Length();
    }
    
    FString Message4 = FString::Printf(TEXT("Ball SPEED - %f"), TimeToBallDest);
    GEngine->AddOnScreenDebugMessage(33, 1, FColor::Green, Message4);


    return FVector::Zero();
}
