// Fill out your copyright notice in the Description page of Project Settings.

#include "Components/TBTextRenderComponent.h"
#include "Components\TBPlayerStateComponent.h"

UTBTextRenderComponent::UTBTextRenderComponent()
{
    SetTextRenderColor(FColor::Green);
    SetHorizontalAlignment(EHorizTextAligment::EHTA_Center);
    SetWorldSize(50.0f);
    SetRelativeLocation(FVector(0.0f, 0.0f, 200.0f));
}

void UTBTextRenderComponent::UpdateTextComponent(const EPlayerState& State)
{
    switch (State)
    {
        case EPlayerState::PassBall:
        {
            SetText(FText::FromString("PASS BALL"));
            break;
        }
        case EPlayerState::TakePassingBall:
        {
            SetText(FText::FromString("TAKE PASSING BALL"));
            break;
        }
        case EPlayerState::RandomRunning:
        {
            SetText(FText::FromString("RANDOM RUNING"));
            break;
        }
        case EPlayerState::MoveToBallAndControl:
        {
            SetText(FText::FromString("Move To Ball And Control"));
            break;
        }
        case EPlayerState::MoveToBallAndShooting:
        {
            SetText(FText::FromString("Move To Ball And Shooting"));
            break;
        }
        case EPlayerState::Wait:
        {
            SetText(FText::FromString("WAIT"));
            break;
        }
        default: break;
    }
}
