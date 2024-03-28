// A test task by KEFIR
#include "UI/MyHUD.h"
#include "UI/Settings/HeaderMenuTabs/VideoSettings/VideoSettingsWidget.h"
#include "UI/Settings/HeaderMenuTabs/AudioSettings/AudioSettingsWidget.h"
#include "GameFramework/PlayerController.h"

void AMyHUD::BeginPlay()
{
    Super::BeginPlay();

    checkf(VideoSettingsWidgetClass, TEXT("VideoSettingsWidgetClass isn't set!"));
    checkf(AudioSettingsWidgetClass, TEXT("AudioSettingsWidgetClass isn't set!"));
}

//----------------------
//  Settings Menu
//----------------------

template <typename T>
void OpenSettingWidget(AMyHUD* MyHud, T*& WidgetPtr, UClass* Class)
{
    if (!IsValid(MyHud)) return;

    MyHud->CollapseAllSettingsMenu();
    if (!IsValid(WidgetPtr))
    {
        WidgetPtr = CreateWidget<T>(MyHud->GetWorld(), Class);
        checkf(IsValid(WidgetPtr), TEXT("Template func OpenSettingWidget is failed, Widget creation failed"));

        WidgetPtr->AddToViewport();
    }
    else
    {
        WidgetPtr->SetVisibility(ESlateVisibility::Visible);
    }

    const auto PlayerController = MyHud->GetOwningPlayerController();
    if (!IsValid(PlayerController)) return;

    FInputModeGameAndUI InputMode;
    InputMode.SetWidgetToFocus(WidgetPtr->TakeWidget());
    InputMode.SetHideCursorDuringCapture(false);

    PlayerController->SetInputMode(InputMode);
}

void AMyHUD::OpenVideoSettings()
{
    OpenSettingWidget<UVideoSettingsWidget>(this, VideoSettingsWidget, VideoSettingsWidgetClass);
}

void AMyHUD::OpenAudioSettings()
{
    OpenSettingWidget<UAudioSettingsWidget>(this, AudioSettingsWidget, AudioSettingsWidgetClass);
}

void AMyHUD::CollapseAllSettingsMenu()
{
    const TArray<UUserWidget*> Settings{VideoSettingsWidget, AudioSettingsWidget};
    for (auto* Setting : Settings)
    {
        if (!IsValid(Setting)) continue;

        Setting->SetVisibility(ESlateVisibility::Collapsed);
    }
}
