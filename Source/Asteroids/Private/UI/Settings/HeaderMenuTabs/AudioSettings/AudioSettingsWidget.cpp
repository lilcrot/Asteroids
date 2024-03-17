// A test task by KEFIR
#include "UI/Settings/HeaderMenuTabs/AudioSettings/AudioSettingsWidget.h"
#include "Settings/MyGameUserSettings.h"
#include "Settings/ScalarGameSettings.h"
#include "Settings/OptionsGameSettings.h"
#include "Components/ListView.h"

DEFINE_LOG_CATEGORY_STATIC(LogAudioSettingsWidget, All, All);


void UAudioSettingsWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();
    checkf(ListViewAudioSettings, TEXT("ListViewAudioSettings doesn't exist!"));
    checkf(ListViewSoundSettings, TEXT("ListViewSoundSettings doesn't exist!"));

    auto* UserSettings = UMyGameUserSettings::Get();
    if (IsValid(UserSettings) == false)
    {
        UE_LOG(LogAudioSettingsWidget, Error, TEXT("MyGameUserSettings is nullptr"));
        return;
    }
    UserSettings->LoadSettings();

    /*---------------- ListViewAudioSettings ----------------*/
    ListViewAudioSettings->ClearListItems();

    const auto AudioSettings = UserSettings->GetAudioSettings();
    for (auto* Setting : AudioSettings)
    {
        ListViewAudioSettings->AddItem(Setting);
    }

    /*---------------- ListViewSoundSettings ----------------*/
    ListViewSoundSettings->ClearListItems();

    const auto* OwningPlayerController = GetOwningPlayer();
    if (IsValid(OwningPlayerController))
    {
        const auto SoundSettings = UserSettings->GetSoundSettings(OwningPlayerController->GetLocalPlayer());
        for (auto* Setting : SoundSettings)
        {
            ListViewSoundSettings->AddItem(Setting);
        }
    }
}
