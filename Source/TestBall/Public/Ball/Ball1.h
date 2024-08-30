// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Ball1.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnBallHit);

class UStaticMeshComponent;
class UTBStaticMeshComponent;

UCLASS()
class TESTBALL_API ABall1 : public AActor
{
    GENERATED_BODY()

public:
    // Sets default values for this actor's properties
    ABall1();
    UTBStaticMeshComponent* GetStaticMeshComponent() { return StaticMeshComponent; }

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Components")
    UTBStaticMeshComponent* StaticMeshComponent;


public:
    // Called every frame
    virtual void Tick(float DeltaTime) override;

    UFUNCTION()
    void HandleOnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse,
        const FHitResult& Hit);

    FOnBallHit OnBallHit;

};
