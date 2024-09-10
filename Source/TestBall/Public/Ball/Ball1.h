// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Ball1.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnBallPassed);
DECLARE_MULTICAST_DELEGATE(FOnBallTaked);

class UStaticMeshComponent;
class UTBStaticMeshComponent;

UCLASS()
class TESTBALL_API ABall1 : public AActor
{
    GENERATED_BODY()

public:

    ABall1();
    UTBStaticMeshComponent* GetStaticMeshComponent() { return StaticMeshComponent; }

    UFUNCTION(BlueprintCallable, Category = "BallPhysic")
    FVector GetBallPhysicVelocity();

    UFUNCTION()
    void HandleOnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse,
        const FHitResult& Hit);

    FOnBallPassed OnBallPassed;
    FOnBallTaked OnBallTaked;

protected:
    virtual void BeginPlay() override;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Components")
    UTBStaticMeshComponent* StaticMeshComponent;

public:
    virtual void Tick(float DeltaTime) override;

    
};
