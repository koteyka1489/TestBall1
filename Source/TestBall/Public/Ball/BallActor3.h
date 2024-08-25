// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BallActor3.generated.h"

class UTBStaticMeshComponent;
class USphereComponent;


UCLASS()
class TESTBALL_API ABallActor3 : public AActor
{
	GENERATED_BODY()
	
public:	
	ABallActor3();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
    USphereComponent* SphereComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
    UTBStaticMeshComponent* StaticMeshComponent;



public:	

	virtual void Tick(float DeltaTime) override;

};
