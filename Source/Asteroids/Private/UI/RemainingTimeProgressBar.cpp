// A test task by KEFIR
#include "UI/RemainingTimeProgressBar.h"

DEFINE_LOG_CATEGORY_STATIC(LogRemainingTimeProgressBar, All, All);

void URemainingTimeProgressBar::Tick(const float DeltaTimeSec)
{
    CurrentRemainingTimePercent =
        FMath::FInterpConstantTo(CurrentRemainingTimePercent, bIsInverse ? 0.0f : 1.0f, DeltaTimeSec, InterpSpeed);

    SetPercent(CurrentRemainingTimePercent);
}

void URemainingTimeProgressBar::StartProgressBar(const float InMaxRemainingTimeSec)
{
    if (0.0f >= InMaxRemainingTimeSec)
    {
        UE_LOG(LogRemainingTimeProgressBar, Warning,
            TEXT("Invalid InMaxRemainingTimeSec(%f) must be more than zero so StartProgressBar is failed"), InMaxRemainingTimeSec)
        return;
    }
    MaxRemainingTimeSec = InMaxRemainingTimeSec;
    CurrentRemainingTimePercent = bIsInverse ? 1.0f : 0.0f;

    InterpSpeed = 1.0f / MaxRemainingTimeSec;
}
