// Fill out your copyright notice in the Description page of Project Settings.

#include "Ball/Ball1.h"
#include "Components\SphereComponent.h"
#include "Components\TBStaticMeshComponent.h"
#include "GameFramework\ProjectileMovementComponent.h"
#include "Player/TBPlayer.h"
#include "Cage/Cage.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ABall1::ABall1()
{
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    StaticMeshComponent = CreateDefaultSubobject<UTBStaticMeshComponent>("StaticMeshComponent");
    SetRootComponent(StaticMeshComponent);

    StaticMeshComponent->OnComponentHit.AddDynamic(this, &ABall1::HandleOnHit);
}

// Called when the game starts or when spawned
void ABall1::BeginPlay()
{
    Super::BeginPlay();
}

// Called every frame
void ABall1::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void ABall1::HandleOnHit(
    UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
    auto Player = Cast<ATBPlayer>(OtherActor);
    if (Player && Player->IsRedyToShoot())
    {
        OnBallHit.Broadcast();

        FVector VectorAngularVelocityInDegrees          = StaticMeshComponent->GetPhysicsAngularVelocityInDegrees();
        FVector VectorAngularVelocityInDegreesNormalize = VectorAngularVelocityInDegrees.GetSafeNormal();

        FVector VectorToCage = GetVectorCageLocation() - this->GetActorLocation();
        float XVectorRand    = FMath::FRandRange(-500.0f, 500.0f);
        float YVectorRand    = FMath::FRandRange(0.0f, 100.0f);
        float ZVectorRand    = FMath::FRandRange(300.0f, 600.0f);
        FVector VectorRandOfset(XVectorRand, YVectorRand, ZVectorRand);

        FVector VectorToGoal          = VectorToCage + VectorOffset + VectorRandOfset;
        FVector VectorToGoalNormalize = VectorToGoal.GetSafeNormal();

        StaticMeshComponent->SetPhysicsLinearVelocity(VectorToGoalNormalize * VectorVelocity);
        StaticMeshComponent->SetPhysicsAngularVelocityInDegrees(VectorAngularVelocityInDegreesNormalize * AngularVelocity);
    }
}

FVector ABall1::GetVectorCageLocation()
{
    auto Cage = UGameplayStatics::GetActorOfClass(GetWorld(), ACage::StaticClass());
    if (Cage)
    {
        return Cage->GetActorLocation();
    }

    return FVector::Zero();
}
