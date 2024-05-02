// A test task by KEFIR
#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "BaseSettingsCategoryWidget.generated.h"

class UImage;
class UBorder;

UCLASS(Abstract, NotBlueprintable)
class ASTEROIDS_API UBaseSettingsCategoryWidget : public UUserWidget
{
    GENERATED_BODY()
public:
    UBaseSettingsCategoryWidget(const FObjectInitializer& ObjectInitializer);

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UImage> BackgroundImage = nullptr;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UBorder> MenuBorder = nullptr;
};
