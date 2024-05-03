// A test task by KEFIR
#include "UI/Settings/Collections/AudioSettings/SoundSettingOptionEntryWidget.h"
#include "Settings/OptionsGameSettings.h"

void USoundSettingOptionEntryWidget::NativeOnListItemObjectSet(UObject* ListItemObject)
{
    Super::NativeOnListItemObjectSet(ListItemObject);

    auto* AudioDeviceOutputGameSetting = Cast<UAudioDeviceOutputGameSetting>(ListItemObject);
    if (AudioDeviceOutputGameSetting != nullptr)
    {
        GameSetting = MakeWeakObjectPtr(AudioDeviceOutputGameSetting);
        AudioDeviceOutputGameSetting->OnAudioSettingsUpdated.AddUObject(this, &ThisClass::UpdateTexts);
    }

    checkf(GameSetting.IsValid(), TEXT("SoundGameSetting isn't valid. GameSetting is implied for this class!"));

    UpdateTexts();
}
