// Fill out your copyright notice in the Description page of Project Settings.


#include "Tests/TBTests.h"
#include "CoreMinimal.h"
#include "Misc/AutomationTest.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FTBTestClass, "TestBall.Math.MaxInt",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority);

bool FTBTestClass::RunTest(const FString& Parameters)
{
    FMath::Max(13, 10);
    TestTrue("Compare 2 different positive numbers", FMath::Max(13, 10) == 13);

    return true;
}