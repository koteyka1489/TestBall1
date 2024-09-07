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
            const FRotator Rotation = UKismetMathLibrary::FindLookAtRotation(this->GetActorLocation(), Ball->GetActorLocation());

            PlayerController->SetControlRotation(Rotation);
        }
    }
}
