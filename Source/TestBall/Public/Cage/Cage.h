// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Cage.generated.h"

class UTBStaticMeshComponent;

UCLASS()
class TESTBALL_API ACage : public AActor
{
    GENERATED_BODY()

public:
    ACage();

protected:
    virtual void BeginPlay() override;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Components")
    UTBStaticMeshComponent* StaticMeshComponent;

public:
    virtual void Tick(float DeltaTime) override;
};
