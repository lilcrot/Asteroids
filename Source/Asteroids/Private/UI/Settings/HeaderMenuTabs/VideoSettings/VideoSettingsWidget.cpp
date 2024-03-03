// A test task by KEFIR
#include "UI/Settings/HeaderMenuTabs/VideoSettings/VideoSettingsWidget.h"
#include "UI/Settings/HeaderMenuTabs/VideoSettings/VideoSettingOptionEntryWidget.h"
#include "Settings/MyGameUserSettings.h"
#include "Settings/GameSettings.h"
#include "Components/ListView.h"
#include "Components/Button.h"

DEFINE_LOG_CATEGORY_STATIC(LogVideoSettingsWidget, All, All);

void UVideoSettingsWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    auto* UserSettings = UMyGameUserSettings::Get();
    if (!UserSettings)
    {
        UE_LOG(LogVideoSettingsWidget, Error, TEXT("MyGameUserSettings is nullptr"));
        return;
    }

    UserSettings->LoadSettings();
    const auto VideoSettings = UserSettings->GetVideoSettings();

    checkf(ListViewVideoSettings, TEXT("ListViewVideoSettings doesn't exist!"));
    ListViewVideoSettings->ClearListItems();

    for (auto* Setting : VideoSettings)
    {
        ListViewVideoSettings->AddItem(Setting);
    }

    checkf(RunBenchmarkButton, TEXT("RunBenchmarkButton doesn't exist!"));
    RunBenchmarkButton->OnClicked.AddDynamic(this, &ThisClass::OnBenchmark);

    UserSettings->OnVideoSettingsUpdated.AddUObject(this, &ThisClass::OnVideoSettingsUpdated);
}

void UVideoSettingsWidget::OnBenchmark()
{
    if (auto* UserSettings = UMyGameUserSettings::Get())
    {
        UserSettings->RunBenchmark();
    }
}

void UVideoSettingsWidget::OnVideoSettingsUpdated()
{
    if (!ListViewVideoSettings) return;
    for (auto* GameSetting : ListViewVideoSettings->GetListItems())
    {
        if (auto* SettingOptionWidget = ListViewVideoSettings->GetEntryWidgetFromItem<UVideoSettingOptionEntryWidget>(GameSetting))
        {
            SettingOptionWidget->UpdateTexts();
        }
    }
}