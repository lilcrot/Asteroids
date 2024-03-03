// A test task by KEFIR

#include "Player/SpacecraftPlayerController.h"

ASpacecraftPlayerController::ASpacecraftPlayerController()
{
    PrimaryActorTick.bTickEvenWhenPaused = true;
}

void ASpacecraftPlayerController::ToggleGamePause()
{
    SetPause(!IsPaused());
}

void ASpacecraftPlayerController::OnPossess(APawn* InPawn)
{
    Super::OnPossess(InPawn);

    SetShowMouseCursor(true);
    SetInputMode(FInputModeGameOnly().SetConsumeCaptureMouseDown(false));
}
