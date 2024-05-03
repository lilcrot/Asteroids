// A test task by KEFIR
#pragma once

#include "CoreMinimal.h"
#include "UI/Settings/Collections/BaseSettingPossibleOptionsEntryWidget.h"

#include "VideoSettingOptionEntryWidget.generated.h"

UCLASS(Abstract, Blueprintable)
class ASTEROIDS_API UVideoSettingOptionEntryWidget : public UBaseSettingPossibleOptionsEntryWidget
{
    GENERATED_BODY()

protected:
    virtual void NativeOnListItemObjectSet(UObject* ListItemObject) override;
};
