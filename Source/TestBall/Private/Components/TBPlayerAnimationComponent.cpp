// Fill out your copyright notice in the Description page of Project Settings.

#include "Components/TBPlayerAnimationComponent.h"
#include "Animations\TBShootEndAnimNotify.h"
#include "Animations\TBPassEndAnimNotify.h"
#include "Animations\TBTakeBallEndAnimNotify.h"
#include "Animation/AnimMontage.h"
#include "Ball\Ball1.h"
#include "Player\TBPlayer.h"
#include "Components\TBBallComputeDataComponent.h"

// Sets default values for this component's properties
UTBPlayerAnimationComponent::UTBPlayerAnimationComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void UTBPlayerAnimationComponent::BeginPlay()
{
    Super::BeginPlay();

    Player = Cast<ATBPlayer>(GetOwner());
    if (Player)
    {
        Ball                     = Player->GetBallPtr();
        BallComputeDataComponent = Player->GetBallComputeDataComponent();
    }
    InitAnimationNotify();
}

void UTBPlayerAnimationComponent::PassBall()
{
    if (Ball && PassAnimMontage)
    {
        PassAnimationExecuted = true;
        Player->SetRotationPlayerOnBall();
        PlayAnimMontage(PassAnimMontage);
    }
}

void UTBPlayerAnimationComponent::ShootBall()
{
    if (!Ball || !ShotAnimMontage) return;

    if (BallComputeDataComponent->GetDistanceToBall() <= (ShootTheBallDistance + 50.0f) && !ShootAnimationExecuted)
    {
        ReadyToShoot = true;
        Player->SetRotationPlayerOnBall();
        PlayAnimMontage(ShotAnimMontage);
        ShootAnimationExecuted = true;
    }
}

void UTBPlayerAnimationComponent::TakeBall()
{
    if (Ball && TakeBallAnimMontage && !IsTakeBallAnimationExecuted())
    {
        TakeBallAnimationExecuted = true;
        PlayAnimMontage(TakeBallAnimMontage);
    }
}

float UTBPlayerAnimationComponent::GetDurationTakeBallAnimMontage()
{
    return TakeBallAnimMontage->GetPlayLength();
}

float UTBPlayerAnimationComponent::PlayAnimMontage(UAnimMontage* Montage)
{
    return Player->PlayAnimMontage(Montage);
}

void UTBPlayerAnimationComponent::InitAnimationNotify()
{
    if (!ShotAnimMontage || !PassAnimMontage || !TakeBallAnimMontage) return;
    InitShotAnimNotify();
    InitPassAnimNotify();
    InitTakeBallAnimNotify();
}

void UTBPlayerAnimationComponent::InitShotAnimNotify()
{
    auto& ShotNotifyEvents = ShotAnimMontage->Notifies;
    for (auto& ShotNotifyEvent : ShotNotifyEvents)
    {
        auto ShootEndNotify = Cast<UTBShootEndAnimNotify>(ShotNotifyEvent.Notify);
        if (ShootEndNotify)
        {
            ShootEndNotify->OnNotified.AddUObject(this, &UTBPlayerAnimationComponent::OnShootAnimationFinished);
        }
    }
}

void UTBPlayerAnimationComponent::InitPassAnimNotify()
{
    auto& PassNotifyEvents = PassAnimMontage->Notifies;
    for (auto& PassNotifyEvent : PassNotifyEvents)
    {
        auto PassEndNotify = Cast<UTBPassEndAnimNotify>(PassNotifyEvent.Notify);
        if (PassEndNotify)
        {
            PassEndNotify->OnNotified.AddUObject(this, &UTBPlayerAnimationComponent::OnPassAnimationFinished);
        }
    }
}

void UTBPlayerAnimationComponent::InitTakeBallAnimNotify()
{
    auto& TakeBallNotifyEvents = TakeBallAnimMontage->Notifies;
    for (auto& TakeBallNotifyEvent : TakeBallNotifyEvents)
    {
        auto TakeBallEndNotify = Cast<UTBTakeBallEndAnimNotify>(TakeBallNotifyEvent.Notify);
        if (TakeBallEndNotify)
        {
            TakeBallEndNotify->OnNotified.AddUObject(this, &UTBPlayerAnimationComponent::OnTakeBallAnimationFinished);
        }
    }
}

void UTBPlayerAnimationComponent::OnShootAnimationFinished()
{
    ReadyToShoot           = false;
    ShootAnimationExecuted = false;
}

void UTBPlayerAnimationComponent::OnPassAnimationFinished()
{
    PassAnimationExecuted = false;

    GEngine->AddOnScreenDebugMessage(2, 3, FColor::Red, TEXT("Pass Animation Finished"));
}

void UTBPlayerAnimationComponent::OnTakeBallAnimationFinished()
{
    TakeBallAnimationExecuted = false;
}