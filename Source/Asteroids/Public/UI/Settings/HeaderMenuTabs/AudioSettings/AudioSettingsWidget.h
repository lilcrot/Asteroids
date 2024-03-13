// A test task by KEFIR
#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "AudioSettingsWidget.generated.h"

class UListView;

UCLASS(Abstract)
class ASTEROIDS_API UAudioSettingsWidget : public UUserWidget
{
    GENERATED_BODY()

protected:
    virtual void NativeOnInitialized() override;

    UPROPERTY(meta = (BindWidget))
    UListView* ListViewAudioSettings;

    UPROPERTY(meta = (BindWidget))
    UListView* ListViewSoundSettings;
};
