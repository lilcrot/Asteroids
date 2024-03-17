// A test task by KEFIR
#include "UI/Settings/BaseGameSettingScreenWidget.h"

UBaseGameSettingScreenWidget::UBaseGameSettingScreenWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
    SetIsFocusable(true);
}

FReply UBaseGameSettingScreenWidget::NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
    if (InKeyEvent.GetKey() == ExitInputKey)
    {
        SetVisibility(ESlateVisibility::Collapsed);
    }

    return Super::NativeOnKeyDown(InGeometry, InKeyEvent);
}
