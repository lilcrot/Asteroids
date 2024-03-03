// A test task by KEFIR
#include "UI/Settings/HeaderMenuTabs/BaseSettingOptionEntryWidget.h"

void UBaseSettingOptionEntryWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    checkf(SettingDisplayName, TEXT("SettingDisplayName doesn't exist!"));
}