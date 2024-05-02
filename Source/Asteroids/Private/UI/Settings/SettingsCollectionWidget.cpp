// A test task by KEFIR
#include "UI/Settings/SettingsCollectionWidget.h"
#include "UI/Settings/HeaderMenuTabWidget.h"
#include "UI/Settings/BaseSettingsCategoryWidget.h"
#include "Components/HorizontalBox.h"
#include "Components/VerticalBox.h"
#include "Components/SizeBox.h"

DEFINE_LOG_CATEGORY_STATIC(LogSettingsCollection, All, All);

USettingsCollectionWidget::USettingsCollectionWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
    SetIsFocusable(true);
}

void USettingsCollectionWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();
    {
        checkf(CollectionLinks, TEXT("CollectionLinks doesn't exist!"));
        checkf(SettingScreen, TEXT("SettingScreen doesn't exist!"));
        checkf(SettingsTopHeader, TEXT("SettingsTopHeader doesn't exist!"));
        checkf(CollectionLinks->GetChildrenCount() != 0, TEXT("CollectionLinks is empty, collection must be completed"));

        checkf(SettingScreen->GetChildrenCount() == 1 && Cast<USizeBox>(SettingScreen->GetChildAt(0)) == SettingsTopHeader,
            TEXT("SettingScreen contains only SettingsTopHeader that is size box"));
    }

    for (auto* Widget : CollectionLinks->GetAllChildren())
    {
        auto* HeaderMenyTab = Cast<UHeaderMenuTabWidget>(Widget);
        if (!HeaderMenyTab)
        {
            UE_LOG(LogSettingsCollection, Error, TEXT("CollectionLinks has child that isn't HeaderMenuTabWidget"))
            continue;
        }
        HeaderMenyTab->Init(this);
    }

    OpenFirstSettingsCategory();
}

void USettingsCollectionWidget::SetVisibility(ESlateVisibility InVisibility)
{
    Super::SetVisibility(InVisibility);
    if (InVisibility == ESlateVisibility::Visible)
    {
        OpenFirstSettingsCategory();
    }
}

FReply USettingsCollectionWidget::NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
    if (InKeyEvent.GetKey() == ExitInputKey)
    {
        SetVisibility(ESlateVisibility::Collapsed);
    }

    return Super::NativeOnKeyDown(InGeometry, InKeyEvent);
}

void USettingsCollectionWidget::OpenFirstSettingsCategory()
{
    auto* HeaderMenyTab = Cast<UHeaderMenuTabWidget>(CollectionLinks->GetChildAt(0));
    if (!HeaderMenyTab)
    {
        UE_LOG(LogSettingsCollection, Error, TEXT("CollectionLinks doesn't have HeaderMenuTabWidget at first index"))
        return;
    }

    OpenSettingsCategory(HeaderMenyTab->GetSettingsCategory());
}

void USettingsCollectionWidget::OpenSettingsCategory(UBaseSettingsCategoryWidget* SettingsCategory)
{
    CurrentCollectionWidget = MakeWeakObjectPtr<UBaseSettingsCategoryWidget>(SettingsCategory);
    if (CurrentCollectionWidget.IsValid())
    {
        CurrentCollectionWidget->SetVisibility(ESlateVisibility::Visible);

        SettingScreen->RemoveChildAt(1);
        SettingScreen->AddChild(CurrentCollectionWidget.Get());
    }
}
