// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TBBallComputeDataComponent.generated.h"

class ABall1;
class ATBPlayer;
class UTBStaticMeshComponent;

struct ShootingData
{
    FVector ShootingDirection;
    FVector ShootingRotation;
};

struct PassingData
{
    FVector PassDirection;
    FVector PassRotation;
};

USTRUCT(BlueprintType)
struct FShootingRandoms
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shooting")
    float x = 500.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shooting")
    float y = 100.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shooting")
    float zMin = 300.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shooting")
    float zMax = 1000.0f;
};

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class TESTBALL_API UTBBallComputeDataComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UTBBallComputeDataComponent();

    UFUNCTION(BlueprintCallable, Category = "Ball")
    FVector GetBallLocation();

    UFUNCTION(BlueprintCallable, Category = "Ball")
    float GetDistanceToBall();

    UFUNCTION(BlueprintCallable, Category = "Ball")
    ABall1* GetBallPtr() { return Ball; }

    FVector FindVecMoveToShootBallPosition();
    FVector FindVecMoveToPassBallPosition();

    ShootingData GetShootingData();
    PassingData GetPassingData();

    FVector GetVecPlayerToBall();

    FVector FindCorrectionPlayerPositionForTakeBall();

protected:
    virtual void BeginPlay() override;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ball")
    ABall1* Ball = nullptr;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shooting")
    float ShootingStrench = 5000;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shooting")
    FShootingRandoms ShootingRandoms;

private:
    void CheckBallLocationAndDirection();
    void CheckPlayerToBallDirection();
    void CheckBallLocation();

    bool BallIsForward       = false;
    bool BallIsCloseLocation = false;

    TObjectPtr<ATBPlayer> Player = nullptr;
    

    FVector VectorToBall = FVector::ZeroVector;
};
