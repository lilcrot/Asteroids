// A test task by KEFIR
#include "UI/Settings/Collections/VideoSettings/VideoSettingsWidget.h"
#include "UI/Settings/Collections/VideoSettings/VideoSettingOptionEntryWidget.h"
#include "Settings/MyGameUserSettings.h"
#include "Settings/OptionsGameSettings.h"
#include "Components/ListView.h"
#include "Components/Button.h"

DEFINE_LOG_CATEGORY_STATIC(LogVideoSettingsWidget, All, All);

void UVideoSettingsWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    auto* UserSettings = UMyGameUserSettings::Get();
    if (!IsValid(UserSettings))
    {
        UE_LOG(LogVideoSettingsWidget, Error, TEXT("MyGameUserSettings is nullptr"));
        return;
    }

    UserSettings->LoadSettings();
    const auto& VideoSettings = UserSettings->GetVideoSettings();

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
    auto* UserSettings = UMyGameUserSettings::Get();
    if (UserSettings == nullptr) return;

    UserSettings->RunBenchmark();
}

void UVideoSettingsWidget::OnVideoSettingsUpdated()
{
    for (auto* GameSetting : ListViewVideoSettings->GetListItems())
    {
        auto* SettingOptionWidget = ListViewVideoSettings->GetEntryWidgetFromItem<UVideoSettingOptionEntryWidget>(GameSetting);
        if (SettingOptionWidget == nullptr) continue;

        SettingOptionWidget->UpdateTexts();
    }
}