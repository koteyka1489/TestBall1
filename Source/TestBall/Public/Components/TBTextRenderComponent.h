// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/TextRenderComponent.h"
#include "TBTextRenderComponent.generated.h"

enum class EPlayerState : uint8;

UCLASS()
class TESTBALL_API UTBTextRenderComponent : public UTextRenderComponent
{
    GENERATED_BODY()

public:
    UTBTextRenderComponent();
    void UpdateTextComponent(const EPlayerState& State);
};
