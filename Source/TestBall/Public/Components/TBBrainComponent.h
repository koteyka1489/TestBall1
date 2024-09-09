// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TBBrainComponent.generated.h"

class ACage;
class ATBPlayer;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TESTBALL_API UTBBrainComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UTBBrainComponent();

    ACage* GetOpponentGoalPost() { return OpponentGoalPost; }
    TArray<ATBPlayer*> GetTeam() { return Team; }

protected:
	virtual void BeginPlay() override;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Team")
    int32 NTeam = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GoalPosts")
    ACage* OwnGoalPost = nullptr;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GoalPosts")
    ACage* OpponentGoalPost = nullptr;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Team")
    TArray<ATBPlayer*> Team;

};
