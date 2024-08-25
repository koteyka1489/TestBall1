// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/StaticMeshComponent.h"
#include "TBStaticMeshComponent.generated.h"

/**
 *
 */
UCLASS()
class TESTBALL_API UTBStaticMeshComponent : public UStaticMeshComponent
{
    GENERATED_BODY()

public:
    UTBStaticMeshComponent();
    virtual void BeginPlay() override;



protected:
};
