// A test task by KEFIR
#include "UI/Settings/Collections/BaseSettingEntryWidget.h"

void UBaseSettingEntryWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    checkf(SettingDisplayName, TEXT("SettingDisplayName doesn't exist!"));
}