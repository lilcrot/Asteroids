// A test task by KEFIR
#pragma once
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "GameplayWidget.generated.h"

class UProgressBar;
class UTextBlock;

UCLASS()
class ASTEROIDS_API UGameplayWidget : public UUserWidget
{
    GENERATED_BODY()
protected:
    void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
    virtual void NativeOnInitialized() override;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UProgressBar> WaveTimeLeftProgressBar;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UTextBlock> WaveNumberText;

protected:
    UFUNCTION(BlueprintPure)
    float GetCurrentTimeLeftPercent() const;

    UFUNCTION()
    void OnNewWaveHasStarted(const FCurrentWaveInfo WaveInfo);

private:
    float CurrentTimeLeft = 0.0f;
    float MaxWavePeriodTime = 0.0f;
};
