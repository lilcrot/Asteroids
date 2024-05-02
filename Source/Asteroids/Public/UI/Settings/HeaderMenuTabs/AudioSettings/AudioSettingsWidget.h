// A test task by KEFIR
#pragma once

#include "CoreMinimal.h"
#include "UI/Settings/BaseSettingsCategoryWidget.h"

#include "AudioSettingsWidget.generated.h"

class UListView;

UCLASS(Abstract, Blueprintable)
class ASTEROIDS_API UAudioSettingsWidget : public UBaseSettingsCategoryWidget
{
    GENERATED_BODY()

protected:
    virtual void NativeOnInitialized() override;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UListView> ListViewAudioSettings;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UListView> ListViewSoundSettings;
};
