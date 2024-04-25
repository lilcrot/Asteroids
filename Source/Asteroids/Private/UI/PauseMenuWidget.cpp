// A test task by KEFIR
#include "UI/PauseMenuWidget.h"

void UPauseMenuWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    checkf(OpenSettingsButton, TEXT("OpenSettingsButton doesn't exist"));
    checkf(ResumeButton, TEXT("ResumeButton doesn't exist"));
}