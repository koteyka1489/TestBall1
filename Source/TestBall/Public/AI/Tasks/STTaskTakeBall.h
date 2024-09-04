// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/StateTreeTaskBlueprintBase.h"
#include "STTaskTakeBall.generated.h"

/**
 * 
 */
UCLASS()
class TESTBALL_API USTTaskTakeBall : public UStateTreeTaskBlueprintBase
{
	GENERATED_BODY()

public:
    USTTaskTakeBall(const FObjectInitializer& ObjectInitializer);

protected:
    virtual EStateTreeRunStatus Tick(FStateTreeExecutionContext& Context, const float DeltaTime) override;
};
