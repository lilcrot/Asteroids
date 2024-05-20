// A test task by KEFIR
#include "UI/Settings/Collections/AudioSettings/AudioSettingsWidget.h"
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
    if (!IsValid(UserSettings))
    {
        UE_LOG(LogAudioSettingsWidget, Error, TEXT("MyGameUserSettings is nullptr"));
        return;
    }
    UserSettings->LoadSettings();

    const auto* OwningPlayerController = GetOwningPlayer();
    if (!IsValid(OwningPlayerController)) return;

    const auto LocalPlayer = OwningPlayerController->GetLocalPlayer();

    /*---------------- ListViewAudioSettings ----------------*/
    ListViewAudioSettings->ClearListItems();

    const auto AudioSettings = UserSettings->GetAudioSettings(LocalPlayer);
    for (auto* Setting : AudioSettings)
    {
        ListViewAudioSettings->AddItem(Setting);
    }

    /*---------------- ListViewSoundSettings ----------------*/
    ListViewSoundSettings->ClearListItems();

    const auto SoundSettings = UserSettings->GetSoundSettings(LocalPlayer);
    for (auto* Setting : SoundSettings)
    {
        ListViewSoundSettings->AddItem(Setting);
    }
}
