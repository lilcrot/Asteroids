// A test task by KEFIR

#include "Player/SpacecraftPlayerController.h"

ASpacecraftPlayerController::ASpacecraftPlayerController()
{
    PrimaryActorTick.bTickEvenWhenPaused = true;
    SetShowMouseCursor(true);
}

void ASpacecraftPlayerController::ToggleGamePause()
{
    const bool bIsPaused = !IsPaused();
    if (!SetPause(bIsPaused)) return;

    if (bIsPaused)
    {
        FInputModeGameAndUI InputMode;
        InputMode.SetHideCursorDuringCapture(false);

        SetInputMode(InputMode);
    }
    else
    {
        FInputModeGameOnly InputMode;
        InputMode.SetConsumeCaptureMouseDown(false);

        SetInputMode(InputMode);
    }
}
