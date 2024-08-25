// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Ball1.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnBallHit);

class UStaticMeshComponent;
class UProjectileMovementComponent;
class UTBStaticMeshComponent;

UCLASS()
class TESTBALL_API ABall1 : public AActor
{
    GENERATED_BODY()

public:
    // Sets default values for this actor's properties
    ABall1();

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Components")
    UTBStaticMeshComponent* StaticMeshComponent;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Physics")
    FVector ImpulseDirection = FVector(0, 0, 1);

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Physics")
    float VectorVelocity = 100.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Physics")
    float AngularVelocity = 100.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Physics")
    FVector VectorOffset{0.0f, 0.0f, 200.0f};

public:
    // Called every frame
    virtual void Tick(float DeltaTime) override;

    UFUNCTION()
    void HandleOnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse,
        const FHitResult& Hit);

    FOnBallHit OnBallHit;

private:
    FVector GetVectorCageLocation();
};
