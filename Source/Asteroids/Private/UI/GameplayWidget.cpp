// A test task by KEFIR
#include "UI/GameplayWidget.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "EnemyWavesGameModeBase.h"

void UGameplayWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();
    {
        checkf(WaveTimeLeftProgressBar, TEXT("WaveTimeLeftProgressBar doesn't exist"));
        checkf(WaveNumberText, TEXT("WaveNumberText doesn't exist"));
    }

    UWorld* World = GetWorld();
    if (!World) return;

    auto* GameMode = Cast<AEnemyWavesGameModeBase>(World->GetAuthGameMode());
    if (GameMode)
    {
        GameMode->OnNewWaveHasStarted.AddDynamic(this, &ThisClass::OnNewWaveHasStarted);
        OnNewWaveHasStarted(GameMode->GetCurrentWaveInfo());
    }
}

void UGameplayWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
    Super::NativeTick(MyGeometry, InDeltaTime);

    CurrentTimeLeft = FMath::Clamp(CurrentTimeLeft - InDeltaTime, 0.0f, MaxWavePeriodTime);
}

float UGameplayWidget::GetCurrentTimeLeftPercent() const
{
    if (FMath::IsNearlyZero(MaxWavePeriodTime) || FMath::IsNearlyZero(CurrentTimeLeft)) return 0.0f;
    return CurrentTimeLeft / MaxWavePeriodTime;
}

void UGameplayWidget::OnNewWaveHasStarted(const FCurrentWaveInfo WaveInfo)
{
    WaveNumberText->SetText(FText::AsNumber(WaveInfo.WaveNumber));

    MaxWavePeriodTime = WaveInfo.EnemyWavePeriodTime;
    CurrentTimeLeft = MaxWavePeriodTime;
}
