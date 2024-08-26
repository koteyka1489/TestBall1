// Fill out your copyright notice in the Description page of Project Settings.

#include "Player/TBPlayer.h"
#include "Camera/CameraComponent.h"
#include "GameFramework\SpringArmComponent.h"
#include "Components/SphereComponent.h"
#include "Components\TBCharacterMovementComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Animation/AnimMontage.h"
#include "Animations/TBAnimNotify.h"
#include "Ball\Ball1.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/PlayerController.h"

class APlayerController;

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
}

FVector ATBPlayer::GetClosebleBallLocation()
{
    if (!ClosebleBall) return FVector::Zero();

    return ClosebleBall->GetActorLocation();
}

float ATBPlayer::GetDistanceToCloseballBall()
{
    return (GetClosebleBallLocation() - GetActorLocation()).Length();
}

bool ATBPlayer::GetPlayerHitedBall()
{
    return IsPlayerHitedBall;
}

bool ATBPlayer::SetPlayerHitedBall(bool in)
{
    return IsPlayerHitedBall = in;
}

// Called when the game starts or when spawned
void ATBPlayer::BeginPlay()
{
    Super::BeginPlay();

    UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABall1::StaticClass(), Balls);
    InitAnimationNotify();
    SetClosebleBall();
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

// Called to bind functionality to input
void ATBPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);
    PlayerInputComponent->BindAxis("MoveForward", this, &ATBPlayer::MoveForward);
    PlayerInputComponent->BindAxis("MoveRight", this, &ATBPlayer::MoveRight);
    PlayerInputComponent->BindAxis("LookUp", this, &ATBPlayer::AddControllerPitchInput);
    PlayerInputComponent->BindAxis("LookRight", this, &ATBPlayer::AddControllerYawInput);
    PlayerInputComponent->BindAction("Shoot", EInputEvent::IE_Pressed, this, &ATBPlayer::Shoot);
}

void ATBPlayer::MoveForward(float Amount)
{
    AddMovementInput(GetActorForwardVector(), Amount);
}

void ATBPlayer::MoveRight(float Amount)
{
    AddMovementInput(GetActorRightVector(), Amount);
}

void ATBPlayer::Shoot()
{
    SetClosebleBall();

    VectorToBall    = ClosebleBall->GetActorLocation() - this->GetActorLocation();
    float VecLenght = VectorToBall.Length();
    IsMovingToBall  = true;
    

    ClosebleBall->OnBallHit.AddUObject(this, &ATBPlayer::OnBallHit);
    if (VecLenght <= ShotTheBallDistance + 20.0f)
    {
        ReadyToShoot = true;
        LockCamera();
        PlayAnimMontage(ShotAnimMontage);
    }
}

void ATBPlayer::CheckBallLocationandDirection(ABall1* Ball)
{
    if (Ball)
    {
        CheckPlayerToBallDirection(Ball);
        CheckBallLocation(Ball);
    }
    else
    {
        checkNoEntry();
        TEXT("Ball not found");
    }
}

void ATBPlayer::CheckPlayerToBallDirection(ABall1* Ball)
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

void ATBPlayer::CheckBallLocation(ABall1* Ball)
{
    VectorToBall    = Ball->GetActorLocation() - this->GetActorLocation();
    float VecLenght = VectorToBall.Length();

    if (VecLenght >= ShotTheBallDistance && VecLenght <= MaxDistanceToMoveTheBall)
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
    VectorToBall = ClosebleBall->GetActorLocation() - this->GetActorLocation();
    if (VectorToBall.Length() > ShotTheBallDistance)
    {
        AddMovementInput(VectorToBall.GetSafeNormal(), GetCharacterMovement()->GetMaxSpeed());
    }
    else
    {
        IsMovingToBall = false;
        GetCharacterMovement()->StopMovementImmediately();
    }
}

void ATBPlayer::InitAnimationNotify()
{

    if (!ShotAnimMontage) return;

    auto NotifyEvents = ShotAnimMontage->Notifies;

    for (auto& NotifyEvent : NotifyEvents)
    {
        auto ShootEndNotify = Cast<UTBAnimNotify>(NotifyEvent.Notify);
        if (ShootEndNotify)
        {
            ShootEndNotify->OnNotified.AddUObject(this, &ATBPlayer::OnShootAnimationFinished);
        }
    }
}

void ATBPlayer::OnShootAnimationFinished()
{
    GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Blue, FString("Shooting AnimationEnd"));
    ReadyToShoot = false;
}

void ATBPlayer::SetClosebleBall()
{
    for (auto& Actor : Balls)
    {
        auto Ball = Cast<ABall1>(Actor);

        if (Ball)
        {
            CheckBallLocationandDirection(Ball);
        }

        if (BallIsCloseLocation && BallIsForward)
        {
            if (!ClosebleBall)
            {
                ClosebleBall = Ball;
            }
            else
            {
                float LenghtOfVectorToClosebleBall = (ClosebleBall->GetActorLocation() - this->GetActorLocation()).Length();
                float LenghtOfVectorToNewBall      = (Ball->GetActorLocation() - this->GetActorLocation()).Length();

                if (LenghtOfVectorToNewBall < LenghtOfVectorToClosebleBall)
                {
                    ClosebleBall = Ball;
                }
            }
        }
    }
}

void ATBPlayer::OnBallHit()
{
    IsPlayerHitedBall = true;
}

void ATBPlayer::LockCamera()
{

    if (LockCameraOnShoot)
    {
        APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);

        if (PlayerController)
        {
            const FRotator Rotation = UKismetMathLibrary::FindLookAtRotation(this->GetActorLocation(), ClosebleBall->GetActorLocation());

            PlayerController->SetControlRotation(Rotation);
        }
    }
}
