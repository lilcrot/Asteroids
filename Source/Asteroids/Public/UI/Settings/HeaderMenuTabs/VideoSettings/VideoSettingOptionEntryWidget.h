// A test task by KEFIR

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI/Settings/HeaderMenuTabs/BaseSettingOptionEntryWidget.h"

#include "VideoSettingOptionEntryWidget.generated.h"

class UTextBlock;
class UVideoGameSetting;
class UButton;
class UGameSettingRotatorWidget;

UCLASS(Abstract)
class ASTEROIDS_API UVideoSettingOptionEntryWidget : public UBaseSettingOptionEntryWidget
{
    GENERATED_BODY()

protected:
    virtual void NativeOnListItemObjectSet(UObject* ListItemObject) override;

    UPROPERTY(meta = (BindWidget))
    UButton* NextSettingButton;

    UPROPERTY(meta = (BindWidget))
    UButton* PrevSettingButton;

    UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
    UGameSettingRotatorWidget* RotatorSettingValue;

    virtual void NativeOnInitialized() override;

    void UpdateTexts();
private:
    TWeakObjectPtr<UVideoGameSetting> GameSetting;

    UFUNCTION()
    void OnNextSetting();

    UFUNCTION()
    void OnPrevSetting();

    friend class UVideoSettingsWidget;
};
