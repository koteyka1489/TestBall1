// Fill out your copyright notice in the Description page of Project Settings.


#include "TBGameModeBase.h"
#include "Player/TBPlayer.h"
#include "Player/TBPlayerController.h"

ATBGameModeBase::ATBGameModeBase() 
{
    DefaultPawnClass = ATBPlayer::StaticClass();
    PlayerControllerClass = ATBPlayerController::StaticClass();
}
