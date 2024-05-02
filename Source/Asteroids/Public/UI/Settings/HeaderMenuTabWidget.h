// A test task by KEFIR
#pragma once
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "HeaderMenuTabWidget.generated.h"

class UButton;
class UTextBlock;
class UBaseSettingsCategoryWidget;

UCLASS()
class ASTEROIDS_API UHeaderMenuTabWidget : public UUserWidget
{
    GENERATED_BODY()
    friend class USettingsCollectionWidget;

protected:
    virtual void NativeOnInitialized() override;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UTextBlock> CollectionNameText;

    UPROPERTY(EditInstanceOnly)
    FText CollectionName;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UButton> OpenTabMenuButton;

    UPROPERTY(EditInstanceOnly)
    TSubclassOf<UBaseSettingsCategoryWidget> SettingsCategoryClass;

private:
    void Init(USettingsCollectionWidget* Collection);

    UFUNCTION()
    void OpenSettingsCategory();
    UBaseSettingsCategoryWidget* GetSettingsCategory();

    UPROPERTY()
    TObjectPtr<UBaseSettingsCategoryWidget> SettingsCategory;

    UPROPERTY()
    TWeakObjectPtr<USettingsCollectionWidget> SettingsCollectionWidget;
};
