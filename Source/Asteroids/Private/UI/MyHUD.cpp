// A test task by KEFIR
#include "UI/MyHUD.h"
#include "UI/Settings/Collections/VideoSettings/VideoSettingsWidget.h"
#include "UI/Settings/Collections/AudioSettings/AudioSettingsWidget.h"
#include "UI/Settings/SettingsCollectionWidget.h"
#include "GameFramework/PlayerController.h"
#include "UI/GameplayWidget.h"
#include "UI/PauseMenuWidget.h"
#include "EnemyWavesGameModeBase.h"

void AMyHUD::BeginPlay()
{
    Super::BeginPlay();
    {
        checkf(PauseMenuWidgetClass, TEXT("PauseMenuWidgetClass isn't set!"));
        checkf(GameplayWidgetClass, TEXT("GameplayWidgetClass isn't set!"));
        checkf(SettingsCollectionWidgetClass, TEXT("SettingsCollectionWidgetClass isn't set!"));
    }

    UWorld* World = GetWorld();
    if (!World) return;

    auto* GameMode = Cast<AEnemyWavesGameModeBase>(World->GetAuthGameMode());
    if (GameMode)
    {
        GameMode->OnGamePauseChangedEvent.AddDynamic(this, &ThisClass::OnGamePauseChanged);
    }

    OpenGameplayWidget();
}

template <typename T>
void SetNewCurrentWidget(AMyHUD* MyHud, TObjectPtr<T>& NewWidget, TSubclassOf<T> Class)
{
    if (!IsValid(MyHud) || !Class || (MyHud->CurrentWidget && MyHud->CurrentWidget == NewWidget)) return;

    if (!NewWidget)
    {
        NewWidget = CreateWidget<T>(MyHud->GetWorld(), Class);
        checkf(NewWidget, TEXT("SetNewCurrentWidget is failed because NewWidget(%s) creation failed"), *Class->GetAuthoredName());
        NewWidget->AddToViewport();
    }
    else
    {
        NewWidget->SetVisibility(ESlateVisibility::Visible);
    }

    if (MyHud->CurrentWidget)
    {
        MyHud->CurrentWidget->SetVisibility(ESlateVisibility::Collapsed);
    }

    MyHud->CurrentWidget = NewWidget;
}

//----------------------
//  GameInProgress
//----------------------

void AMyHUD::OpenGameplayWidget()
{
    SetNewCurrentWidget<UGameplayWidget>(this, GameplayWidget, GameplayWidgetClass);
}

//----------------------
//  GameInPause
//----------------------

template <typename T>
void OpenGameInPauseWidget(AMyHUD* MyHud, TObjectPtr<T>& WidgetPtr, TSubclassOf<T> Class)
{
    if (!IsValid(MyHud)) return;
    SetNewCurrentWidget<T>(MyHud, WidgetPtr, Class);

    if (!WidgetPtr) return;
    WidgetPtr->SetFocus();
}

void AMyHUD::OpenSettingsCollection()
{
    OpenGameInPauseWidget<USettingsCollectionWidget>(this, SettingsCollectionWidget, SettingsCollectionWidgetClass);
}

void AMyHUD::OnGamePauseChanged(const bool bIsPaused)
{
    if (bIsPaused)
    {
        OpenGameInPauseWidget<UPauseMenuWidget>(this, PauseMenuWidget, PauseMenuWidgetClass);
    }
    else
    {
        OpenGameplayWidget();
    }
}
