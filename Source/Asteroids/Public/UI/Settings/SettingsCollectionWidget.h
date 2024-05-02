// A test task by KEFIR
#pragma once
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "SettingsCollectionWidget.generated.h"

class UHorizontalBox;
class UVerticalBox;
class USizeBox;

class UBaseSettingsCategoryWidget;

UCLASS()
class ASTEROIDS_API USettingsCollectionWidget : public UUserWidget
{
    GENERATED_BODY()
    friend class UHeaderMenuTabWidget;

public:
    USettingsCollectionWidget(const FObjectInitializer& ObjectInitializer);

    virtual void SetVisibility(ESlateVisibility InVisibility) override;

protected:
    virtual void NativeOnInitialized() override;

    virtual FReply NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SettingsCollection")
    FKey ExitInputKey = "Escape";

protected:
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UHorizontalBox> CollectionLinks;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UVerticalBox> SettingScreen;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<USizeBox> SettingsTopHeader;

private:
    UPROPERTY()
    TWeakObjectPtr<UBaseSettingsCategoryWidget> CurrentCollectionWidget;

    void OpenFirstSettingsCategory();
    void OpenSettingsCategory(UBaseSettingsCategoryWidget* SettingsCategory);
};
