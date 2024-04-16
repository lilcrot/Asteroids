// A test task by KEFIR
#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/IUserObjectListEntry.h"

#include "BaseSettingEntryWidget.generated.h"

class UTextBlock;

UCLASS(Abstract, Blueprintable)
class ASTEROIDS_API UBaseSettingEntryWidget : public UUserWidget, public IUserObjectListEntry
{
    GENERATED_BODY()

protected:
    virtual void NativeOnInitialized() override;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UTextBlock> SettingDisplayName;

    UFUNCTION(BlueprintImplementableEvent)
    void OnValueChanged(const float NewValue);

    /* Note: INDEX_NONE(-1) will hide default option */
    UFUNCTION(BlueprintImplementableEvent)
    void OnDefaultValueChanged(const float DefaultValue);
};
