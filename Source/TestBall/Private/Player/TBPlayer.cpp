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
#include "Components\TBTextRenderComponent.h"

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
    TextRenderComponent      = CreateDefaultSubobject<UTBTextRenderComponent>("UTextRenderComponent");
    TextRenderComponent->SetupAttachment(GetRootComponent());

    AutoPossessAI     = EAutoPossessAI::PlacedInWorldOrSpawned;
    AIControllerClass = ATBAIController::StaticClass();
    
    
}

void ATBPlayer::BeginPlay()
{
    Super::BeginPlay();
    GetCharacterMovement()->bOrientRotationToMovement = false;
    bUseControllerRotationYaw                         = false;

    TextRenderComponent->UpdateTextComponent(PlayerStateComponent->GetPlayerState());
    ATBAIController* AIController = Cast<ATBAIController>(this->GetController());
    if (AIController)
    {
        AIController->ReceiveMoveCompleted.AddDynamic(this, &ATBPlayer::OnMoveCompleted);
    }
}

void ATBPlayer::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    TextRenderComponent->UpdateTextComponent(PlayerStateComponent->GetPlayerState());
    if (bMoveToTargetLeftOrRightStrafe) MoveToTargetLeftOrRightStrafeTick();
}


bool ATBPlayer::IsMoveToLocationComplete()
{
    return bMoveToLocationComplete;
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


ABall1* ATBPlayer::GetBallPtr()
{
    return BallComputeDataComponent->GetBallPtr();
}



void ATBPlayer::OnMoveCompleted(FAIRequestID RequestID, EPathFollowingResult::Type Result)
{
    bMoveToLocationComplete = true;
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

void ATBPlayer::MoveToLocation(FVector TargetLocation)
{
    float TimeToLocation = GetTimeMoveToLocation(TargetLocation);
    GEngine->AddOnScreenDebugMessage(3, 5.0f, FColor::Green, FString::Printf(TEXT("TIME to Location - %f"), TimeToLocation));

    BallComputeDataComponent->GetBallLocationInTime(TimeToLocation);


    bMoveToLocationComplete       = false;
    ATBAIController* AIController = Cast<ATBAIController>(this->GetController());
    if (AIController)
    {
        AIController->MoveToLocation(TargetLocation);
    }
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

float ATBPlayer::GetTimeMoveToLocation(FVector Location)
{
    FVector VecToTarget = Location - GetActorLocation();
    float LengthVecToTarget = VecToTarget.Length();
    float PlayerSpeed       = GetMovementComponent()->GetMaxSpeed();
    return LengthVecToTarget / PlayerSpeed;
}
