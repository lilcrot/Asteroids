// A test task by KEFIR
#include "UI/Settings/HeaderMenuTabWidget.h"
#include "UI/Settings/SettingsCollectionWidget.h"
#include "UI/Settings/BaseSettingsCategoryWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"

void UHeaderMenuTabWidget::Init(USettingsCollectionWidget* Collection)
{
    SettingsCollectionWidget = MakeWeakObjectPtr<USettingsCollectionWidget>(Collection);

    checkf(SettingsCollectionWidget.IsValid(), TEXT("Init is failed, SettingsCollectionWidget isn't valid"));
}

void UHeaderMenuTabWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();
    checkf(OpenTabMenuButton, TEXT("OpenTabMenuButton doesn't exist!"));
    checkf(CollectionNameText, TEXT("CollectionNameText doesn't exist!"));
    checkf(IsValid(SettingsCategoryClass), TEXT("SettingsCategoryClass isn't valid"));

    CollectionNameText->SetText(CollectionName);

    OpenTabMenuButton->OnClicked.AddDynamic(this, &ThisClass::OpenSettingsCategory);
}

void UHeaderMenuTabWidget::OpenSettingsCategory()
{
    if (SettingsCollectionWidget.IsValid())
    {
        SettingsCollectionWidget->OpenSettingsCategory(GetSettingsCategory());
    }
}

UBaseSettingsCategoryWidget* UHeaderMenuTabWidget::GetSettingsCategory()
{
    if (!IsValid(SettingsCategory))
    {
        SettingsCategory = CreateWidget<UBaseSettingsCategoryWidget>(GetWorld(), SettingsCategoryClass);
        checkf(IsValid(SettingsCategory), TEXT("SettingsCategory creation is failed"));
        SettingsCategory->AddToViewport();
    }

    return SettingsCategory;
}
