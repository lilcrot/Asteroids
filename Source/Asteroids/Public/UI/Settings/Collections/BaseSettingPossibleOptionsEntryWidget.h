// A test task by KEFIR

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI/Settings/Collections/BaseSettingEntryWidget.h"

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
    TObjectPtr<UButton> NextSettingButton;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UButton> PrevSettingButton;

    UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
    TObjectPtr<UGameSettingRotatorWidget> RotatorSettingValue;

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
