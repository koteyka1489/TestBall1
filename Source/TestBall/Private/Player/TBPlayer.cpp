// Fill out your copyright notice in the Description page of Project Settings.

#include "Player/TBPlayer.h"
#include "Camera/CameraComponent.h"
#include "GameFramework\SpringArmComponent.h"
#include "Components/SphereComponent.h"
#include "Components\TBCharacterMovementComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Animation/AnimMontage.h"
#include "Animations\TBShootEndAnimNotify.h"
#include "Animations\TBPassEndAnimNotify.h"
#include "Ball\Ball1.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/PlayerController.h"
#include "Components\TBStaticMeshComponent.h"
#include "Cage/Cage.h"
#include "AI\TBAIPlayer.h"

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

    float GoalVecLenght = VecToBallLenght - ShootTheBallDistance + (ShootTheBallDistance / 3);

    FVector VecToBallNormalize = VecToBall.GetSafeNormal();

    FVector Result = VecToBallNormalize * GoalVecLenght;

    return Result;
}

// Called when the game starts or when spawned
void ATBPlayer::BeginPlay()
{
    Super::BeginPlay();

    InitAnimationNotify();
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
    PlayerInputComponent->BindAction("Shoot", EInputEvent::IE_Pressed, this, &ATBPlayer::MoveToBallAndShoot);
}

void ATBPlayer::MoveForward(float Amount)
{
    AddMovementInput(GetActorForwardVector(), Amount);
}

void ATBPlayer::MoveRight(float Amount)
{
    AddMovementInput(GetActorRightVector(), Amount);
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

bool ATBPlayer::Shoot(float VecToBallLenght)
{
    if (VecToBallLenght <= ShootTheBallDistance + 50.0f && !ShootAnimationExecuted)
    {
        if (Ball && ShotAnimMontage)
        {
            Ball->OnBallHit.AddUObject(this, &ATBPlayer::OnBallHit);
            ReadyToShoot = true;
            LockCamera();
            PlayAnimMontage(ShotAnimMontage);
            ShootAnimationExecuted = true;
            return true;
        }
    }
    return false;
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

    if (VecLenght >= ShootTheBallDistance && VecLenght <= MaxDistanceToMoveTheBall)
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
    if (VectorToBall.Length() > ShootTheBallDistance)
    {
        AddMovementInput(VectorToBall.GetSafeNormal(), GetCharacterMovement()->GetMaxSpeed());
    }
    else
    {
        IsMovingToBall = false;
        GetCharacterMovement()->StopMovementImmediately();
    }
}

void ATBPlayer::PassBall(float VecToBallLenght)
{
    if (Ball && PassAnimMontage)
    {
        PassAnimationExecuted = true;
        Ball->OnBallHit.AddUObject(this, &ATBPlayer::OnBallHit);
        LockCamera();
        PlayAnimMontage(PassAnimMontage);
    }
}

void ATBPlayer::InitAnimationNotify()
{

    if (!ShotAnimMontage) return;

    auto NotifyEvents = ShotAnimMontage->Notifies;

    for (auto& NotifyEvent : NotifyEvents)
    {
        auto ShootEndNotify = Cast<UTBShootEndAnimNotify>(NotifyEvent.Notify);
        if (ShootEndNotify)
        {
            ShootEndNotify->OnNotified.AddUObject(this, &ATBPlayer::OnShootAnimationFinished);
        }

        auto PassEndNotify = Cast<UTBPassEndAnimNotify>(NotifyEvent.Notify);
        if (PassEndNotify)
        {
            PassEndNotify->OnNotified.AddUObject(this, &ATBPlayer::OnPassAnimationFinished);
        }
    }
}

void ATBPlayer::OnShootAnimationFinished()
{
    ReadyToShoot           = false;
    ShootAnimationExecuted = false;
}

void ATBPlayer::OnPassAnimationFinished()
{
    PassAnimationExecuted = false;
}

void ATBPlayer::CheckMoveToBall()
{
    if (Ball)
    {
        VectorToBall    = Ball->GetActorLocation() - this->GetActorLocation();
        float VecLenght = VectorToBall.Length();
        if (VecLenght > ShootTheBallDistance)
        {
            IsMovingToBall = true;
        }
    }
}

void ATBPlayer::MoveToBallAndShoot()
{
    CheckMoveToBall();
    Shoot(GetDistanceToBall());
}

void ATBPlayer::OnBallHit() {}

void ATBPlayer::LockCamera()
{

    if (LockCameraOnShoot)
    {
        APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);

        if (PlayerController)
        {
            const FRotator Rotation = UKismetMathLibrary::FindLookAtRotation(this->GetActorLocation(), Ball->GetActorLocation());

            PlayerController->SetControlRotation(Rotation);
        }
    }
}
