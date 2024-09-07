// Fill out your copyright notice in the Description page of Project Settings.

#include "Player/TBPlayer.h"
#include "Camera/CameraComponent.h"
#include "GameFramework\SpringArmComponent.h"
#include "Components/SphereComponent.h"
#include "Components\TBCharacterMovementComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Ball\Ball1.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/PlayerController.h"
#include "Components\TBStaticMeshComponent.h"
#include "Cage/Cage.h"
#include "AI\TBAIPlayer.h"
#include "Components\TBPlayerAnimationComponent.h"

class APlayerController;
class UTBStaticMeshComponent;

// Sets default values
ATBPlayer::ATBPlayer()
{
    // Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>("SpringArmComponent");
    SpringArmComponent->SetupAttachment(GetRootComponent());
    SpringArmComponent->bUsePawnControlRotation = true;

    CameraComponent = CreateDefaultSubobject<UCameraComponent>("CameraComponent");
    CameraComponent->SetupAttachment(SpringArmComponent);

    PlayerAnimationComponent = CreateDefaultSubobject<UTBPlayerAnimationComponent>("PLayerAnimationComponent");
}

FVector ATBPlayer::GetBallLocation()
{
    if (!Ball) return FVector::Zero();

    return Ball->GetActorLocation();
}

float ATBPlayer::GetDistanceToBall()
{
    return (GetBallLocation() - GetActorLocation()).Length();
}

FVector ATBPlayer::FindVecMoveToShootBallPosition()
{
    FVector VecToBall = GetBallLocation() - GetActorLocation();

    float VecToBallLenght = VecToBall.Length();

    float GoalVecLenght =
        VecToBallLenght - PlayerAnimationComponent->GetShootTheBallDistance() + (PlayerAnimationComponent->GetShootTheBallDistance() / 3);

    FVector VecToBallNormalize = VecToBall.GetSafeNormal();

    FVector Result = VecToBallNormalize * GoalVecLenght;

    return Result;
}

FVector ATBPlayer::FindVecMoveToPassBallPosition()
{
    FVector VecToBall = GetBallLocation() - GetActorLocation();

    float VecToBallLenght = VecToBall.Length();

    float GoalVecLenght =
        VecToBallLenght - PlayerAnimationComponent->GetPassBallDistance() + (PlayerAnimationComponent->GetPassBallDistance() / 3);

    FVector VecToBallNormalize = VecToBall.GetSafeNormal();

    FVector Result = VecToBallNormalize * GoalVecLenght;

    return Result;
}

// Called when the game starts or when spawned
void ATBPlayer::BeginPlay()
{
    Super::BeginPlay();
}

// Called every frame
void ATBPlayer::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (IsMovingToBall)
    {
        MoveToBall();
    }
}


ShootingData ATBPlayer::GetShootingData()
{
    ShootingData Result;

    FVector VectorAngularVelocityInDegrees = Ball->GetStaticMeshComponent()->GetPhysicsAngularVelocityInDegrees();
    Result.ShootingRotation                = VectorAngularVelocityInDegrees.GetSafeNormal();

    FVector CageLocation = OpponentGoalPost->GetActorLocation();

    FVector VectorToCage = OpponentGoalPost->GetActorLocation() - this->GetActorLocation();

    float XRand = FMath::FRandRange(-ShootingRandoms.x, ShootingRandoms.x);
    float YRand = FMath::FRandRange(-ShootingRandoms.y, ShootingRandoms.y);
    float ZRand = FMath::FRandRange(ShootingRandoms.zMin, ShootingRandoms.zMax);
    FVector RandOffset(XRand, YRand, ZRand);

    FVector VectorToGoal = VectorToCage + RandOffset;

    FVector VectorToGoalNormalize = VectorToGoal.GetSafeNormal();
    Result.ShootingDirection      = VectorToGoalNormalize * ShootingStrench;

    return Result;
}

PassingData ATBPlayer::GetPassingData()
{
    PassingData Result{};

    if (Team.Num() > 0)
    {
        Result.PassDirection = Team[0]->GetActorLocation() - this->GetActorLocation();
        Result.PassRotation  = FVector::Zero();
        return Result;
    }

    return Result;
}

void ATBPlayer::CheckBallLocationAndDirection()
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

void ATBPlayer::CheckPlayerToBallDirection()
{
    VectorToBall                   = Ball->GetActorLocation() - this->GetActorLocation();
    float DotProductBallAndePlayer = FVector::DotProduct(VectorToBall, this->GetActorForwardVector());
    if (DotProductBallAndePlayer > 0.5)
    {
        BallIsForward = true;
    }
    else
    {
        BallIsForward = false;
    }
}

void ATBPlayer::CheckBallLocation()
{
    VectorToBall    = Ball->GetActorLocation() - this->GetActorLocation();
    float VecLenght = VectorToBall.Length();

    if (VecLenght >= PlayerAnimationComponent->GetShootTheBallDistance() && VecLenght <= MaxDistanceToMoveTheBall)
    {
        BallIsCloseLocation = true;
    }
    else
    {
        BallIsCloseLocation = false;
    }
}

void ATBPlayer::MoveToBall()
{
    VectorToBall = Ball->GetActorLocation() - this->GetActorLocation();
    if (VectorToBall.Length() > PlayerAnimationComponent->GetShootTheBallDistance())
    {
        AddMovementInput(VectorToBall.GetSafeNormal(), GetCharacterMovement()->GetMaxSpeed());
    }
    else
    {
        IsMovingToBall = false;
        PlayerReadyToShoot = true;
        GetCharacterMovement()->StopMovementImmediately();
    }
}

void ATBPlayer::CheckMoveToBall()
{
    if (Ball)
    {
        VectorToBall    = Ball->GetActorLocation() - this->GetActorLocation();
        float VecLenght = VectorToBall.Length();
        if (VecLenght > PlayerAnimationComponent->GetShootTheBallDistance())
        {
            IsMovingToBall = true;
        }
    }
}

void ATBPlayer::MoveToBallAndShoot()
{
    CheckMoveToBall();
    PlayerAnimationComponent->Shoot(GetDistanceToBall());
}

void ATBPlayer::OnBallHit() {}

void ATBPlayer::SetRotationPlayerOnBall()
{

    if (bSetRotationPlayerOnBall)
    {
        APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);

        if (PlayerController)
        {
            const FRotator Rotation = UKismetMathLibrary::FindLookAtRotation(this->GetActorLocation(), Ball->GetActorLocation());

            PlayerController->SetControlRotation(Rotation);
        }
    }
}
