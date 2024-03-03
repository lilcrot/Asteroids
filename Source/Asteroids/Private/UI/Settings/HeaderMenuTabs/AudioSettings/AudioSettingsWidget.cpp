// A test task by KEFIR
#include "UI/Settings/HeaderMenuTabs/AudioSettings/AudioSettingsWidget.h"
#include "Settings/MyGameUserSettings.h"
#include "Settings/GameSettings.h"
#include "Components/ListView.h"

DEFINE_LOG_CATEGORY_STATIC(LogAudioSettingsWidget, All, All);

void UAudioSettingsWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();
    checkf(ListViewAudioSettings, TEXT("ListViewAudioSettings doesn't exist!"));

    auto* UserSettings = UMyGameUserSettings::Get();
    if (!UserSettings)
    {
        UE_LOG(LogAudioSettingsWidget, Error, TEXT("MyGameUserSettings is nullptr"));
        return;
    }
    UserSettings->LoadSettings();

    ListViewAudioSettings->ClearListItems();
    const auto AudioSettings = UserSettings->GetAudioSettings();
    for (auto* Setting : AudioSettings)
    {
        ListViewAudioSettings->AddItem(Setting);
    }
}
