// A test task by KEFIR
#include "UI/Settings/HeaderMenuTabs/VideoSettings/VideoSettingOptionEntryWidget.h"
#include "Settings/GameSettings.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "UI/Settings/HeaderMenuTabs/VideoSettings/GameSettingRotatorWidget.h"

void UVideoSettingOptionEntryWidget::NativeOnListItemObjectSet(UObject* ListItemObject)
{
    Super::NativeOnListItemObjectSet(ListItemObject);

    auto* EntryInfo = Cast<UVideoGameSetting>(ListItemObject);
    GameSetting = MakeWeakObjectPtr(EntryInfo);
    checkf(GameSetting.IsValid(), TEXT("VideoGameSetting isn't valid. GameSetting is implied for this class!"));

    {
        TArray<FText> Options;
        for (const auto& Opt : GameSetting->GetPossibleOptions())
        {
            Options.Add(Opt.QualityName);
        }
        RotatorSettingValue->PopulateTextLabels(Options);
        OnDefaultValueChanged(INDEX_NONE);
    }

    UpdateTexts();
}

void UVideoSettingOptionEntryWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    checkf(NextSettingButton, TEXT("NextSettingButton doesn't exist!"));
    checkf(PrevSettingButton, TEXT("PrevSettingButton doesn't exist!"));
    checkf(RotatorSettingValue, TEXT("RotatorSettingValue doesn't exist!"));

    NextSettingButton->OnClicked.AddDynamic(this, &ThisClass::OnNextSetting);
    PrevSettingButton->OnClicked.AddDynamic(this, &ThisClass::OnPrevSetting);
}

void UVideoSettingOptionEntryWidget::UpdateTexts()
{
    if (GameSetting.IsValid())
    {
        SettingDisplayName->SetText(GameSetting->GetName());
        RotatorSettingValue->SetSelectedItem(GameSetting->GetCurrentOptionIndex());
    }
}

void UVideoSettingOptionEntryWidget::OnNextSetting()
{
    if (GameSetting.IsValid())
    {
        RotatorSettingValue->ShiftTextRight();
        GameSetting->ApplyNextOption();
    }
}

void UVideoSettingOptionEntryWidget::OnPrevSetting()
{
    if (GameSetting.IsValid())
    {
        RotatorSettingValue->ShiftTextLeft();
        GameSetting->ApplyPrevOption();
    }
}