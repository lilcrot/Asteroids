// A test task by KEFIR
#pragma once

#include "CoreMinimal.h"
#include "InputAction.h"
#include "InputTriggers.h"

#include "InputRecordingTypes.generated.h"

class UInputAction;

USTRUCT()
struct FActionData
{
    GENERATED_BODY()

    UPROPERTY()
    FName ActionName;

    UPROPERTY()
    FVector Value = FVector::ZeroVector;

    UPROPERTY()
    ETriggerEvent CurrentTrigger = ETriggerEvent::None;
};

USTRUCT()
struct FBindingsData
{
    GENERATED_BODY()

    UPROPERTY()
    TArray<FActionData> ActionsData;

    UPROPERTY()
    float WorldTime = 0.0f;
};

USTRUCT()
struct FInputData
{
    GENERATED_BODY()

    UPROPERTY()
    TArray<FBindingsData> Bindings;

    UPROPERTY()
    FTransform InitialTransform;
};
