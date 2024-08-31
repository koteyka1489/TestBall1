// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/StateTreeTaskBlueprintBase.h"
#include "STTaskRotateToBall.generated.h"

/**
 * 
 */
UCLASS()
class TESTBALL_API USTTaskRotateToBall : public UStateTreeTaskBlueprintBase
{
	GENERATED_BODY()
	
	public:
    USTTaskRotateToBall(const FObjectInitializer& ObjectInitializer);

	protected:
    virtual EStateTreeRunStatus Tick(FStateTreeExecutionContext& Context, const float DeltaTime) override;
};
