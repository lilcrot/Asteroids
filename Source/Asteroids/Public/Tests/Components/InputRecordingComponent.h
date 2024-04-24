// A test task by KEFIR
#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Tests/InputRecordingTypes.h"

#include "InputRecordingComponent.generated.h"

/*------------InputRecordingComponent------------
 *
 * This component save all enhanced inputs from devices(e.g keyboard) to json file with parametrized name,
 * to "Saved/Tests/{FileName}.json" path by default.
 * It can use in automation tests, e.g in PlayerTests
 *
 *------------InputRecordingComponent------------*/

class UEnhancedPlayerInput;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class ASTEROIDS_API UInputRecordingComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UInputRecordingComponent();

protected:
    virtual void BeginPlay() override;
    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

    UPROPERTY(EditAnywhere)
    FString FileName{"CharacterTestInput"};

public:
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
    UPROPERTY()
    TObjectPtr<UEnhancedPlayerInput> EnhancedPlayerInput;

    FInputData InputData;

    FBindingsData MakeBindingsData() const;
    FString GenerateFileName() const;
};
