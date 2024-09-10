// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/StateTreeTaskBlueprintBase.h"
#include "STTaskTBMoveToBall.generated.h"

/**
 *
 */
UCLASS()
class TESTBALL_API USTTaskTBMoveToBall : public UStateTreeTaskBlueprintBase
{
    GENERATED_BODY()

public:
    USTTaskTBMoveToBall(const FObjectInitializer& ObjectInitializer);

protected:
    virtual EStateTreeRunStatus Tick(FStateTreeExecutionContext& Context, const float DeltaTime) override;
};
