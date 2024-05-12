// A test task by KEFIR
#pragma once
#include "CoreMinimal.h"
#include "Components/ProgressBar.h"

#include "RemainingTimeProgressBar.generated.h"

UCLASS()
class ASTEROIDS_API URemainingTimeProgressBar : public UProgressBar
{
    GENERATED_BODY()

public:
    /* Note: must be called from the owner */
    void Tick(const float DeltaTimeSec);
    void StartProgressBar(const float InMaxRemainingTimeSec);

    /* True - from 100% to 0% | False - from 0% to 100%  */
    UPROPERTY(EditAnywhere)
    bool bIsInverse = true;

private:
    float MaxRemainingTimeSec = 0.0f;
    float CurrentRemainingTimePercent = 0.0f;

    float InterpSpeed = 0.0f;
};
