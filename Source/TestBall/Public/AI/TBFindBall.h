// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/StateTreeTaskBlueprintBase.h"
#include "TBFindBall.generated.h"

/**
 *
 */
UCLASS()
class TESTBALL_API UTBFindBall : public UStateTreeTaskBlueprintBase
{
    GENERATED_BODY()

public:
    UTBFindBall(const FObjectInitializer& ObjectInitializer);

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Output")
    FVector ClosestBallLocation = FVector::Zero();

protected:
    virtual EStateTreeRunStatus EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) override;
    virtual EStateTreeRunStatus Tick(FStateTreeExecutionContext& Context, const float DeltaTime) override;

};
