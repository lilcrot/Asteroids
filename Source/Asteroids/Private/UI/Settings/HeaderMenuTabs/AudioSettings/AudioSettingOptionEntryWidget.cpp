// A test task by KEFIR
#include "UI/Settings/HeaderMenuTabs/AudioSettings/AudioSettingOptionEntryWidget.h"
#include "Settings/GameSettings.h"
#include "Components/TextBlock.h"
#include "Components/Slider.h"

void UAudioSettingOptionEntryWidget::NativeOnListItemObjectSet(UObject* ListItemObject)
{
    Super::NativeOnListItemObjectSet(ListItemObject);

    auto* EntryInfo = Cast<UScalarGameSetting>(ListItemObject);
    GameSetting = MakeWeakObjectPtr(EntryInfo);
    checkf(GameSetting.IsValid(), TEXT("ScalarGameSetting isn't valid. GameSetting is implied for this class!"));

    {
        SettingDisplayName->SetText(GameSetting->GetName());
        OnDefaultValueChanged(-1.0f);
        UpdateTexts();
    }
}

void UAudioSettingOptionEntryWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();
    checkf(SliderSettingValue, TEXT("SliderSettingValue doesn't exist!"));
    checkf(PercentageValueText, TEXT("PercentageValueText doesn't exist!"));

    SliderSettingValue->OnValueChanged.AddDynamic(this, &ThisClass::HandleSliderValueChanged);
}

void UAudioSettingOptionEntryWidget::HandleSliderValueChanged(const float NewValue)
{
    if (GameSetting.IsValid())
    {
        GameSetting->SetValue(NewValue);
        UpdateTexts();
    }
}

void UAudioSettingOptionEntryWidget::UpdateTexts()
{
    const float CurrentVolume = GameSetting->GetCurrentValue();
    PercentageValueText->SetText(FText::AsPercent(CurrentVolume));
    OnValueChanged(CurrentVolume);
}
