// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Ball.generated.h"

class UStaticMeshComponent;
class USceneComponent;

UCLASS()
class TESTBALL_API ABall : public AActor
{
    GENERATED_BODY()

public:
    ABall();

protected:
    virtual void BeginPlay() override;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Components")
    USceneComponent* SceneComponent;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Components")
    UStaticMeshComponent* StaticMeshComponent;

public:
    virtual void Tick(float DeltaTime) override;
};
