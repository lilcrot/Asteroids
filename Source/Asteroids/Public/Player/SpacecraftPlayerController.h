// A test task by KEFIR

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"

#include "SpacecraftPlayerController.generated.h"

UCLASS()
class ASTEROIDS_API ASpacecraftPlayerController : public APlayerController
{
    GENERATED_BODY()

public:
    ASpacecraftPlayerController();

    UFUNCTION(BlueprintCallable, Category = "PlayerController")
    void ToggleGamePause();

protected:
    virtual void OnPossess(APawn* InPawn) override;
};
