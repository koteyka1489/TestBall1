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
#include "AI\TBAIPlayer.h"
#include "Components\TBPlayerAnimationComponent.h"
#include "Components\TBBallComputeDataComponent.h"

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
    BallComputeDataComponent = CreateDefaultSubobject<UTBBallComputeDataComponent>("BallComputeDataComponent");
}

bool ATBPlayer::IsCanMakePass()
{
    return BallComputeDataComponent->GetDistanceToBall() < (PlayerAnimationComponent->GetPassBallDistance() + 50.0);
}

FVector ATBPlayer::FindVecMoveToPassBallPosition()
{
    return GetActorLocation() + BallComputeDataComponent->FindVecMoveToPassBallPosition();
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
    return DistanceToBall < 800.0f && DistanceToBall > 400.0f && !PlayerAnimationComponent->IsTakeBallAnimationExecuted();
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

    float VecLenght      = BallComputeDataComponent->GetVecPlayerToBall().Length();
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
            const FRotator Rotation = UKismetMathLibrary::FindLookAtRotation(this->GetActorLocation(), BallComputeDataComponent->GetBallLocation());

            PlayerController->SetControlRotation(Rotation);
        }
    }
}
