// A test task by KEFIR
#include "UI/Settings/Collections/VideoSettings/VideoSettingOptionEntryWidget.h"
#include "Settings/OptionsGameSettings.h"

void UVideoSettingOptionEntryWidget::NativeOnListItemObjectSet(UObject* ListItemObject)
{
    Super::NativeOnListItemObjectSet(ListItemObject);

    GameSetting = MakeWeakObjectPtr(Cast<UVideoGameSetting>(ListItemObject));
    checkf(GameSetting.IsValid(), TEXT("VideoGamseSetting isn't valid. GameSetting is implied for this class!"));

    UpdateTexts();
}
