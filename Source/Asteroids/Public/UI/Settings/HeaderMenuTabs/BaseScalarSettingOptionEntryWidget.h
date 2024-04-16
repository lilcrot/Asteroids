// A test task by KEFIR
#pragma once

#include "CoreMinimal.h"
#include "UI/Settings/HeaderMenuTabs/BaseSettingEntryWidget.h"

#include "BaseScalarSettingOptionEntryWidget.generated.h"

class UTextBlock;
class USlider;

class UScalarGameSetting;

UCLASS(Abstract)
class ASTEROIDS_API UBaseScalarSettingOptionEntryWidget : public UBaseSettingEntryWidget
{
    GENERATED_BODY()

protected:
    virtual void NativeOnListItemObjectSet(UObject* ListItemObject) override;

    virtual void NativeOnInitialized() override;

    UFUNCTION()
    void HandleSliderValueChanged(const float NewValue);

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<USlider> SliderSettingValue;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UTextBlock> ScalarValueText;

private:
    TWeakObjectPtr<UScalarGameSetting> GameSetting;

    void UpdateTexts();
};
