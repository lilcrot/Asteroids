// A test task by KEFIR
#pragma once

#include "CoreMinimal.h"
#include "UI/Settings/BaseSettingsCategoryWidget.h"

#include "VideoSettingsWidget.generated.h"

class UListView;
class UButton;

UCLASS(Abstract, Blueprintable)
class ASTEROIDS_API UVideoSettingsWidget : public UBaseSettingsCategoryWidget
{
    GENERATED_BODY()

protected:
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UListView> ListViewVideoSettings;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UButton> RunBenchmarkButton;

    virtual void NativeOnInitialized() override;

private:
    UFUNCTION()
    void OnBenchmark();

    void OnVideoSettingsUpdated();
};
