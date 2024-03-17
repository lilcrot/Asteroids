// A test task by KEFIR
#include "UI/Settings/HeaderMenuTabs/BaseScalarSettingOptionEntryWidget.h"
#include "Settings/ScalarGameSettings.h"
#include "Components/TextBlock.h"
#include "Components/Slider.h"

void UBaseScalarSettingOptionEntryWidget::NativeOnListItemObjectSet(UObject* ListItemObject)
{
    Super::NativeOnListItemObjectSet(ListItemObject);

    auto* EntryInfo = Cast<UScalarGameSetting>(ListItemObject);
    GameSetting = MakeWeakObjectPtr(EntryInfo);
    checkf(GameSetting.IsValid(), TEXT("ScalarGameSetting isn't valid. GameSetting is implied for this class!"));

    SettingDisplayName->SetText(GameSetting->GetName());
    OnDefaultValueChanged(INDEX_NONE);
    UpdateTexts();
}

void UBaseScalarSettingOptionEntryWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();
    checkf(SliderSettingValue, TEXT("SliderSettingValue doesn't exist!"));
    checkf(ScalarValueText, TEXT("ScalarValueText doesn't exist!"));

    SliderSettingValue->OnValueChanged.AddDynamic(this, &ThisClass::HandleSliderValueChanged);
}

void UBaseScalarSettingOptionEntryWidget::HandleSliderValueChanged(const float NewValue)
{
    if (GameSetting.IsValid())
    {
        GameSetting->SetValue(NewValue);
        UpdateTexts();
    }
}

void UBaseScalarSettingOptionEntryWidget::UpdateTexts()
{
    if (GameSetting.IsValid())
    {
        const float CurrentValue = GameSetting->GetCurrentValue();

        switch (GameSetting->GetDisplayFormat())
        {
            case (EScalarDisplayFormat::SDF_Number):
            {
                ScalarValueText->SetText(FText::AsNumber(CurrentValue));
                break;
            }
            case (EScalarDisplayFormat::SDF_Percent):
            {
                ScalarValueText->SetText(FText::AsPercent(CurrentValue));
                break;
            }
        }

        OnValueChanged(CurrentValue);
    }
}
