// A test task by KEFIR
#pragma once
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "PauseMenuWidget.generated.h"

class UButton;

UCLASS()
class ASTEROIDS_API UPauseMenuWidget : public UUserWidget
{
    GENERATED_BODY()
protected:
    virtual void NativeOnInitialized() override;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UButton> OpenSettingsButton;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UButton> ResumeButton;
};
