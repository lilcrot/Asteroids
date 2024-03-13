// A test task by KEFIR
#include "UI/Settings/HeaderMenuTabs/BaseSettingPossibleOptionsEntryWidget.h"
#include "Settings/OptionsGameSettings.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "UI/Settings/GameSettingRotatorWidget.h"

void UBaseSettingPossibleOptionsEntryWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    checkf(NextSettingButton, TEXT("NextSettingButton doesn't exist!"));
    checkf(PrevSettingButton, TEXT("PrevSettingButton doesn't exist!"));
    checkf(RotatorSettingValue, TEXT("RotatorSettingValue doesn't exist!"));

    NextSettingButton->OnClicked.AddDynamic(this, &ThisClass::OnNextSetting);
    PrevSettingButton->OnClicked.AddDynamic(this, &ThisClass::OnPrevSetting);
}

void UBaseSettingPossibleOptionsEntryWidget::UpdateTexts()
{
    if (GameSetting.IsValid())
    {
        SettingDisplayName->SetText(GameSetting->GetName());

        RotatorSettingValue->PopulateTextLabels(GameSetting->GetPossibleOptions());
        RotatorSettingValue->SetSelectedItem(GameSetting->GetCurrentOptionIndex());

        OnDefaultValueChanged(INDEX_NONE);
    }
}

void UBaseSettingPossibleOptionsEntryWidget::OnNextSetting()
{
    if (GameSetting.IsValid())
    {
        RotatorSettingValue->ShiftTextRight();
        GameSetting->ApplyNextOption();
    }
}

void UBaseSettingPossibleOptionsEntryWidget::OnPrevSetting()
{
    if (GameSetting.IsValid())
    {
        RotatorSettingValue->ShiftTextLeft();
        GameSetting->ApplyPrevOption();
    }
}