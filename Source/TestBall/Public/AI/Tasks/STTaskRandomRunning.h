// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/StateTreeTaskBlueprintBase.h"
#include "STTaskRandomRunning.generated.h"

/**
 * 
 */
UCLASS()
class TESTBALL_API USTTaskRandomRunning : public UStateTreeTaskBlueprintBase
{
	GENERATED_BODY()

    USTTaskRandomRunning(const FObjectInitializer& ObjectInitializer);

protected:
    virtual EStateTreeRunStatus EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) override;
    virtual EStateTreeRunStatus Tick(FStateTreeExecutionContext& Context, const float DeltaTime) override;

    private:
    FVector NewLocation = {0.0f, 0.0f, 0.0f};
};
