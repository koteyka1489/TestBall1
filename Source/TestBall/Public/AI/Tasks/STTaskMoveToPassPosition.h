// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/StateTreeTaskBlueprintBase.h"
#include "STTaskMoveToPassPosition.generated.h"

/**
 * 
 */
UCLASS()
class TESTBALL_API USTTaskMoveToPassPosition : public UStateTreeTaskBlueprintBase
{
	GENERATED_BODY()

public:
    USTTaskMoveToPassPosition(const FObjectInitializer& ObjectInitializer);

protected:
    virtual EStateTreeRunStatus EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) override;
    virtual EStateTreeRunStatus Tick(FStateTreeExecutionContext& Context, const float DeltaTime) override;

private:
    FVector TargetLocation = FVector::Zero();
};
