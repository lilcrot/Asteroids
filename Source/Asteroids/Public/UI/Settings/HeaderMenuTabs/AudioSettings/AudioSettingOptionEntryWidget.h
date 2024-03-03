// A test task by KEFIR
#pragma once

#include "CoreMinimal.h"
#include "UI/Settings/HeaderMenuTabs/BaseSettingOptionEntryWidget.h"

#include "AudioSettingOptionEntryWidget.generated.h"

class UTextBlock;
class USlider;

class UScalarGameSetting;

UCLASS(Abstract)
class ASTEROIDS_API UAudioSettingOptionEntryWidget : public UBaseSettingOptionEntryWidget
{
    GENERATED_BODY()

protected:
    virtual void NativeOnListItemObjectSet(UObject* ListItemObject) override;

    virtual void NativeOnInitialized() override;

    UFUNCTION()
    void HandleSliderValueChanged(const float NewValue);

    UPROPERTY(meta = (BindWidget))
    USlider* SliderSettingValue;

    UPROPERTY(meta = (BindWidget))
    UTextBlock* PercentageValueText;

private:
    TWeakObjectPtr<UScalarGameSetting> GameSetting;

    void UpdateTexts();
};
