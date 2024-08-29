// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "TBAnimNotify.generated.h"

DECLARE_MULTICAST_DELEGATE(FONNotifiedSignature);

UCLASS()
class TESTBALL_API UTBAnimNotify : public UAnimNotify
{
    GENERATED_BODY()

public:
    virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference);

    FONNotifiedSignature OnNotified;
};
