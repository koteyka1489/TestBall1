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
#include "Ball\Ball1.h"
#include "Components\TextRenderComponent.h"

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
    TextRenderComponent      = CreateDefaultSubobject<UTextRenderComponent>("UTextRenderComponent");
    TextRenderComponent->SetupAttachment(GetRootComponent());

    AutoPossessAI     = EAutoPossessAI::PlacedInWorldOrSpawned;
    AIControllerClass = ATBAIController::StaticClass();
    InitTextRenderComponent();
    UpdateTextComponent();
}

void ATBPlayer::BeginPlay()
{
    Super::BeginPlay();
    GetCharacterMovement()->bOrientRotationToMovement = false;
    bUseControllerRotationYaw                         = false;
}

void ATBPlayer::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    UpdateTextComponent();

    if (bMoveToBallForShootingPos) MoveToBallForShootiongPosTick();

    if (bMoveToTargetLeftOrRightStrafe) MoveToTargetLeftOrRightStrafeTick();

    if (bMoveToTarget) MoveToTargetTick();

    if (bMoveToBall) MoveToBallTick();
}

bool ATBPlayer::IsCanMakePass()
{
    return bMoveToPassEnd;
}

bool ATBPlayer::IsCanMakeShoot()
{
    return BallComputeDataComponent->GetDistanceToBall() < (PlayerAnimationComponent->GetShootTheBallDistance() + 50.0f);
}

bool ATBPlayer::IsCanTakeBall()
{
    float DistanceToBall = BallComputeDataComponent->GetDistanceToBall();

    return DistanceToBall < MaxDistanceToStartTakeBall && DistanceToBall > MinDistanceToStartTakeBall &&
           !PlayerAnimationComponent->IsTakeBallAnimationExecuted();
}

bool ATBPlayer::IsTakeBallComplete()
{
    return PlayerAnimationComponent->IsTakeBallAnimationExecuted() && IsPlayerHaveBall();
}

bool ATBPlayer::IsMoveToBallComplete()
{
    return BallComputeDataComponent->GetDistanceToBall() < 100.0;
}

ABall1* ATBPlayer::GetBallPtr()
{
    return BallComputeDataComponent->GetBallPtr();
}

void ATBPlayer::MoveToBallForShootiongPosTick()
{
    FVector VectorToBall = BallComputeDataComponent->GetVecPlayerToBall();
    if (VectorToBall.Length() > PlayerAnimationComponent->GetShootTheBallDistance())
    {
        AddMovementInput(VectorToBall.GetSafeNormal(), GetCharacterMovement()->GetMaxSpeed());
    }
    else
    {
        bMoveToBallForShootingPos = false;
        PlayerReadyToShoot        = true;
        GetCharacterMovement()->StopMovementImmediately();
    }
}

void ATBPlayer::CheckMoveToBallForShootingPos()
{

    float VecLenght = BallComputeDataComponent->GetVecPlayerToBall().Length();
    if (VecLenght > PlayerAnimationComponent->GetShootTheBallDistance())
    {
        bMoveToBallForShootingPos = true;
    }
}

void ATBPlayer::MoveToBallAndShoot()
{
    CheckMoveToBallForShootingPos();
    PlayerAnimationComponent->ShootBall();
}

void ATBPlayer::MoveToTargetTick()
{
    FVector Direction          = MoveToTargetPositionVec - GetActorLocation();
    FVector DirectionNormalize = Direction.GetSafeNormal();
    
    AddMovementInput(DirectionNormalize, 1.0f);

    if (Direction.Length() < MoveToTargetGoalLenght + 35.0f)
    {
        GetCharacterMovement()->StopMovementImmediately();
        MoveToTargetPositionVec = FVector::Zero();
        bMoveToTarget           = false;
        bMoveToPassEnd          = true;
    }
}

void ATBPlayer::MoveToBallTick()
{
    FString Message = FString::Printf(TEXT("BAll Postion - %s"), *BallComputeDataComponent->GetBallLocation().ToString());
    GEngine->AddOnScreenDebugMessage(8, 1.0f, FColor::Black, Message);

    FVector DirectionToBall          = BallComputeDataComponent->GetBallLocation() - GetActorLocation();
    FVector DirectionToBallNormalize = DirectionToBall.GetSafeNormal();

    AAIController* AIController = Cast<AAIController>(GetController());
    if (AIController)
    {
        AIController->MoveToLocation(DirectionToBall);
    }

    if (DirectionToBall.Length() < 50.0f)
    {
        GetCharacterMovement()->StopMovementImmediately();
        bMoveToBall    = false;
        bMoveToPassEnd = true;
    }
}

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
    MoveToTargetPositionVec = Location;
    bMoveToTarget           = true;
    bMoveToPassEnd          = false;
    MoveToTargetGoalLenght  = (BallComputeDataComponent->GetBallLocation() - Location).Length();
}

void ATBPlayer::RotateToTarget(FRotator Rotation, float DeltaTime)
{
    FRotator SmoothRotation = FMath::RInterpTo(this->GetActorRotation(), Rotation, DeltaTime, RotationSpeed);
    SetActorRotation(SmoothRotation);
}

void ATBPlayer::MoveToTargetLeftOrRightStrafe(FVector Location)
{
    MoveToTargetNoRotVec           = Location;
    bMoveToTargetLeftOrRightStrafe = true;
}

void ATBPlayer::OnBallPassed()
{
    SetPlayerHaveBall(false);
    MessageToPassedPlayer();
    this->PlayerStateComponent->SetPlayerState(EPlayerState::Wait);
}

void ATBPlayer::OnBallTaked()
{
    SetPlayerHaveBall(true);
    this->PlayerStateComponent->SetPlayerState(EPlayerState::PassBall);
}

void ATBPlayer::MoveToBallForShootiongPos()
{
    bMoveToBallForShootingPos = true;
}

void ATBPlayer::MoveToBall()
{
    bMoveToBall    = true;
    bMoveToPassEnd = false;
}

void ATBPlayer::MessageToPassedPlayer()
{
    auto PassedPlayer = BrainComponent->GetPassedPlayer();
    PassedPlayer->GetPlayerStateComponent()->SetPlayerState(EPlayerState::TakePassingBall);
}

void ATBPlayer::InitTextRenderComponent()
{
    TextRenderComponent->SetTextRenderColor(FColor::Green);
    TextRenderComponent->SetHorizontalAlignment(EHorizTextAligment::EHTA_Center);
    TextRenderComponent->SetWorldSize(50.0f);
    TextRenderComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 200.0f));
}

void ATBPlayer::UpdateTextComponent()
{
    switch (PlayerStateComponent->GetPlayerState())
    {
        case EPlayerState::PassBall:
        {
            TextRenderComponent->SetText(FText::FromString("PASS BALL"));
            break;
        }
        case EPlayerState::TakePassingBall:
        {
            TextRenderComponent->SetText(FText::FromString("TAKE PASSING BALL"));
            break;
        }
        case EPlayerState::RandomRunning:
        {
            TextRenderComponent->SetText(FText::FromString("RANDOM RUNING"));
            break;
        }
        case EPlayerState::MoveToBallAndControl:
        {
            TextRenderComponent->SetText(FText::FromString("Move To Ball And Control"));
            break;
        }
        case EPlayerState::MoveToBallAndShooting:
        {
            TextRenderComponent->SetText(FText::FromString("Move To Ball And Shooting"));
            break;
        }
        case EPlayerState::Wait:
        {
            TextRenderComponent->SetText(FText::FromString("WAIT"));
            break;
        }
        default: break;
    }
}

void ATBPlayer::MoveToTargetLeftOrRightStrafeTick()
{
    if ((MoveToTargetNoRotVec - GetActorLocation()).Length() <= 10.0f)
    {
        MoveToTargetNoRotVec           = FVector::Zero();
        bMoveToTargetLeftOrRightStrafe = false;
        GetCharacterMovement()->StopMovementImmediately();
    }

    FVector MoveToTargetNoRotVecNormalize = (MoveToTargetNoRotVec - GetActorLocation()).GetSafeNormal();
    float Dot                             = MoveToTargetNoRotVecNormalize.Dot(GetActorRightVector());

    if (Dot >= 0.1f)
    {
        AddMovementInput(GetActorRightVector(), 1.0f);
    }
    if (Dot <= -0.1f)
    {
        AddMovementInput(GetActorRightVector(), -1.0f);
    }
}
