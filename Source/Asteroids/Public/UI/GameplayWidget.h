// A test task by KEFIR
#pragma once
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "GameplayWidget.generated.h"

class URemainingTimeProgressBar;
class UTextBlock;
class UWeaponComponent;

UCLASS()
class ASTEROIDS_API UGameplayWidget : public UUserWidget
{
    GENERATED_BODY()
protected:
    virtual void NativeOnInitialized() override;
    virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime);

    UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
    TObjectPtr<URemainingTimeProgressBar> WaveRemainingTimeProgressBar;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UTextBlock> WaveNumberText;

    UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
    TObjectPtr<URemainingTimeProgressBar> LaserShotsReloadTimeProgressBar;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UTextBlock> LaserShotsNumberText;

private:
    UFUNCTION()
    void OnNewWaveHasStarted(const FCurrentWaveInfo WaveInfo);

    void SetLaserShotsNumber(const int32 NewLaserShots);
    void OnAllPlayerWeaponsSpawned();

private:
    TWeakObjectPtr<UWeaponComponent> PlayerWeaponComponent;
};
