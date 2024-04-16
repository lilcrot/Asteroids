// A test task by KEFIR
#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "BaseGameSettingScreenWidget.generated.h"

class AMyHUD;
class UImage;
class UBorder;

UCLASS(Abstract, NotBlueprintable)
class ASTEROIDS_API UBaseGameSettingScreenWidget : public UUserWidget
{
    GENERATED_BODY()
public:
    UBaseGameSettingScreenWidget(const FObjectInitializer& ObjectInitializer);

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UImage> BackgroundImage = nullptr;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UBorder> MenuBorder = nullptr;

protected:
    virtual FReply NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "BaseGameSettingScreen")
    FKey ExitInputKey = "Escape";
};
