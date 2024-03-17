// A test task by KEFIR
#include "UI/Settings/HeaderMenuTabs/AudioSettings/SoundSettingOptionEntryWidget.h"
#include "Settings/OptionsGameSettings.h"

void USoundSettingOptionEntryWidget::NativeOnListItemObjectSet(UObject* ListItemObject)
{
    Super::NativeOnListItemObjectSet(ListItemObject);

    if (auto* AudioDeviceOutputGameSetting = Cast<UAudioDeviceOutputGameSetting>(ListItemObject))
    {
        GameSetting = MakeWeakObjectPtr(AudioDeviceOutputGameSetting);
        AudioDeviceOutputGameSetting->OnAudioSettingsUpdated.AddUObject(this, &ThisClass::UpdateTexts);
    }

    checkf(GameSetting.IsValid(), TEXT("SoundGameSetting isn't valid. GameSetting is implied for this class!"));

    UpdateTexts();
}
