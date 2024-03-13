// A test task by KEFIR

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI/Settings/HeaderMenuTabs/BaseSettingEntryWidget.h"

#include "BaseSettingPossibleOptionsEntryWidget.generated.h"

class UTextBlock;
class UBaseOptionGameSetting;
class UButton;
class UGameSettingRotatorWidget;

UCLASS(Abstract)
class ASTEROIDS_API UBaseSettingPossibleOptionsEntryWidget : public UBaseSettingEntryWidget
{
    GENERATED_BODY()

protected:
    UPROPERTY(meta = (BindWidget))
    UButton* NextSettingButton;

    UPROPERTY(meta = (BindWidget))
    UButton* PrevSettingButton;

    UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
    UGameSettingRotatorWidget* RotatorSettingValue;

    virtual void NativeOnInitialized() override;

    void UpdateTexts();

    TWeakObjectPtr<UBaseOptionGameSetting> GameSetting;

private:
    UFUNCTION()
    void OnNextSetting();

    UFUNCTION()
    void OnPrevSetting();

    friend class UVideoSettingsWidget;
};
