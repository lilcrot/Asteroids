// A test task by KEFIR
#include "UI/PauseMenuWidget.h"

UPauseMenuWidget::UPauseMenuWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
    SetIsFocusable(true);
}

void UPauseMenuWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    checkf(OpenSettingsButton, TEXT("OpenSettingsButton doesn't exist"));
    checkf(ResumeButton, TEXT("ResumeButton doesn't exist"));
}