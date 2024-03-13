// A test task by KEFIR
#include "UI/Settings/HeaderMenuTabs/BaseSettingEntryWidget.h"

void UBaseSettingEntryWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    checkf(SettingDisplayName, TEXT("SettingDisplayName doesn't exist!"));
}