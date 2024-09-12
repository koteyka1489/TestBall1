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
    if (IsMovingToBall)
    {
        MoveToBall();
    }
    if (bMoveToTargetNoRot)
    {
        MoveToTargetNoRotationTick();
    }
}

bool ATBPlayer::IsCanMakePass()
{
    return BallComputeDataComponent->GetDistanceToBall() < (PlayerAnimationComponent->GetPassBallDistance() + 50.0f);
}

bool ATBPlayer::IsCanMakeShoot()
{
    return BallComputeDataComponent->GetDistanceToBall() < (PlayerAnimationComponent->GetShootTheBallDistance() + 50.0f);
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

bool ATBPlayer::IsMoveToBallComplete()
{
    return BallComputeDataComponent->GetDistanceToBall() < 100.0;
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
    PlayerAnimationComponent->ShootBall();
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

void ATBPlayer::MoveToTargetNoRotation(FVector Location) 
{
    MoveToTargetNoRotVec = Location;
    bMoveToTargetNoRot   = true;
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

void ATBPlayer::MoveToTargetNoRotationTick() 
{
    float Dot = MoveToTargetNoRotVec.Dot(GetActorRightVector());
    if (Dot >= 0.1f)
    {
        AddMovementInput(GetActorRightVector(), 1.0f);
    }
    if (Dot <= -0.1f)
    {
        AddMovementInput(GetActorRightVector(), -1.0f);
    }

    if (MoveToTargetNoRotVec.Length() <= 1.0f)
    {
        MoveToTargetNoRotVec = FVector::Zero();
        bMoveToTargetNoRot   = false;
    }
 
    //AddMovementInput(MoveToTargetNoRotVec.GetSafeNormal(), GetCharacterMovement()->GetMaxSpeed());
}
