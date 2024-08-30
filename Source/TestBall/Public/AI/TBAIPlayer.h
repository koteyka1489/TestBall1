// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Player/TBPlayer.h"
#include "TBAIPlayer.generated.h"

class ATBAIController;
    /**
 * 
 */
UCLASS()
class TESTBALL_API ATBAIPlayer : public ATBPlayer
{
	GENERATED_BODY()
	
	public:
    ATBAIPlayer();

	void MoveToTarget(FVector Location);
    virtual bool Shoot(float VecToBallLenght) override;


};
