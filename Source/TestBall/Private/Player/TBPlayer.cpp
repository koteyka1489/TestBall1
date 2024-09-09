// Fill out your copyright notice in the Description page of Project Settings.

#include "Player/TBPlayer.h"
#include "Camera/CameraComponent.h"
#include "GameFramework\SpringArmComponent.h"
#include "Components/SphereComponent.h"
#include "Components\TBCharacterMovementComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/PlayerController.h"
#include "Cage/Cage.h"
#include "Components\TBPlayerAnimationComponent.h"
#include "Components\TBBallComputeDataComponent.h"
#include "AI\TBAIController.h"
#include "Components\TBPlayerStateComponent.h"
#include "Components\TBBrainComponent.h"

class APlayerController;
class UTBStaticMeshComponent;

// Sets default values
ATBPlayer::ATBPlayer()
{
    // Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    PlayerAnimationComponent = CreateDefaultSubobject<UTBPlayerAnimationComponent>("PLayerAnimationComponent");
    BallComputeDataComponent = CreateDefaultSubobject<UTBBallComputeDataComponent>("BallComputeDataComponent");
    PlayerStateComponent     = CreateDefaultSubobject<UTBPlayerStateComponent>("PlayerStateComponent");
    BrainComponent           = CreateDefaultSubobject<UTBBrainComponent>("BrainComponent");

    AutoPossessAI     = EAutoPossessAI::PlacedInWorldOrSpawned;
    AIControllerClass = ATBAIController::StaticClass();
}

void ATBPlayer::BeginPlay()
{
    Super::BeginPlay();
}

void ATBPlayer::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    UpdatePlayerState();
    if (IsMovingToBall)
    {
        MoveToBall();
    }
}

bool ATBPlayer::IsCanMakePass()
{
    return BallComputeDataComponent->GetDistanceToBall() < (PlayerAnimationComponent->GetPassBallDistance() + 50.0);
}

FVector ATBPlayer::FindVecMoveToPassBallPosition()
{
    return BallComputeDataComponent->FindVecMoveToPassBallPosition();
}

bool ATBPlayer::IsCanMakeShoot()
{
    return BallComputeDataComponent->GetDistanceToBall() < (PlayerAnimationComponent->GetShootTheBallDistance() + 50.0f);
}

FVector ATBPlayer::FindVecMoveToShootBallPosition()
{
    return GetActorLocation() + BallComputeDataComponent->FindVecMoveToShootBallPosition();
}

void ATBPlayer::PassBall()
{
    PlayerAnimationComponent->PassBall(BallComputeDataComponent->GetDistanceToBall());
}

bool ATBPlayer::IsPassAnimationExecuted()
{
    return PlayerAnimationComponent->IsPassAnimationExecuted();
}

FVector ATBPlayer::GetVectorPlayerToBall()
{
    return BallComputeDataComponent->GetVecPlayerToBall();
}

void ATBPlayer::ShootBall()
{
    PlayerAnimationComponent->Shoot(BallComputeDataComponent->GetDistanceToBall());
}

bool ATBPlayer::IsShootAnimationExecuted()
{
    return PlayerAnimationComponent->IsShootAnimationExecuted();
}

void ATBPlayer::TakeBall()
{
    PlayerAnimationComponent->TakeBall();
}

bool ATBPlayer::IsCanTakeBall()
{
    float DistanceToBall = BallComputeDataComponent->GetDistanceToBall();
    UE_LOG(LogTemp, Display, TEXT("DistanceToBall %f"), DistanceToBall);

    return DistanceToBall < MaxDistanceToStartTakeBall && DistanceToBall > MinDistanceToStartTakeBall &&
           !PlayerAnimationComponent->IsTakeBallAnimationExecuted();
}

bool ATBPlayer::IsTakeBallComplete()
{
    return PlayerAnimationComponent->IsTakeBallAnimationExecuted() && IsPlayerHaveBall();
}

FVector ATBPlayer::GetBallLocation()
{
    return BallComputeDataComponent->GetBallLocation();
}

bool ATBPlayer::IsMoveToBallComplete()
{
    return BallComputeDataComponent->GetDistanceToBall() < 100.0;
}

ShootingData ATBPlayer::GetShootingData()
{
    return BallComputeDataComponent->GetShootingData();
}

PassingData ATBPlayer::GetPassingData()
{
    return BallComputeDataComponent->GetPassingData();
}

ABall1* ATBPlayer::GetBallPtr()
{
    return BallComputeDataComponent->GetBallPtr();
}

void ATBPlayer::MoveToBall()
{
    FVector VectorToBall = BallComputeDataComponent->GetVecPlayerToBall();
    if (VectorToBall.Length() > PlayerAnimationComponent->GetShootTheBallDistance())
    {
        AddMovementInput(VectorToBall.GetSafeNormal(), GetCharacterMovement()->GetMaxSpeed());
    }
    else
    {
        IsMovingToBall     = false;
        PlayerReadyToShoot = true;
        GetCharacterMovement()->StopMovementImmediately();
    }
}

void ATBPlayer::CheckMoveToBall()
{

    float VecLenght = BallComputeDataComponent->GetVecPlayerToBall().Length();
    if (VecLenght > PlayerAnimationComponent->GetShootTheBallDistance())
    {
        IsMovingToBall = true;
    }
}

void ATBPlayer::MoveToBallAndShoot()
{
    CheckMoveToBall();
    PlayerAnimationComponent->Shoot(BallComputeDataComponent->GetDistanceToBall());
}

void ATBPlayer::OnBallHit() {}

void ATBPlayer::SetRotationPlayerOnBall()
{

    if (bSetRotationPlayerOnBall)
    {
        APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);

        if (PlayerController)
        {
            const FRotator Rotation =
                UKismetMathLibrary::FindLookAtRotation(this->GetActorLocation(), BallComputeDataComponent->GetBallLocation());

            PlayerController->SetControlRotation(Rotation);
        }
    }
}

void ATBPlayer::MoveToTarget(FVector Location)
{
    ATBAIController* AiControllerComp = Cast<ATBAIController>(GetController());
    if (AiControllerComp)
    {
        AiControllerComp->MoveToLocation(Location);
    }
}

void ATBPlayer::RotateToTarget(FRotator Rotation, float DeltaTime)
{
    FRotator SmoothRotation = FMath::RInterpTo(this->GetActorRotation(), Rotation, DeltaTime, RotationSpeed);
    SetActorRotation(SmoothRotation);
}

void ATBPlayer::SetPlayerState(EPlayerState State)
{
    PlayerStateComponent->SetPlayerState(State);
}

EPlayerState ATBPlayer::GetPlayerState()
{
    return PlayerStateComponent->GetPlayerState();
}

void ATBPlayer::UpdatePlayerState()
{
    if (IsPlayerHaveBall())
    {
        PlayerStateComponent->SetPlayerState(EPlayerState::PassBall);
    }
    if (!IsPlayerHaveBall())
    {
        PlayerStateComponent->SetPlayerState(EPlayerState::TakePassingBall);
    }
}
