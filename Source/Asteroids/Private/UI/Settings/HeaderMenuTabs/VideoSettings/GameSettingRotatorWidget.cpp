// A test task by KEFIR
#include "UI/Settings/HeaderMenuTabs/VideoSettings/GameSettingRotatorWidget.h"

UGameSettingRotatorWidget::UGameSettingRotatorWidget(const FObjectInitializer& Initializer) : Super(Initializer) {}

void UGameSettingRotatorWidget::SetDefaultOption(const int32 DefaultOptionIndex)
{
    OnDefaultOptionSpecified(DefaultOptionIndex);
}