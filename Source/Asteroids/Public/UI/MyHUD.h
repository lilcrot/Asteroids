// A test task by KEFIR
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"

#include "MyHUD.generated.h"

class UPauseMenuWidget;
class UGameplayWidget;
class USettingsCollectionWidget;
class UGameOverWidget;

UCLASS()
class ASTEROIDS_API AMyHUD : public AHUD
{
    GENERATED_BODY()

protected:
    virtual void BeginPlay() override;

    // @param Class: if the NewWidget does not exist, it will be created using this class
    template <typename T>
    friend void SetNewCurrentWidget(AMyHUD* MyHud, TObjectPtr<T>& NewWidget, TSubclassOf<T> Class);

private:
    TObjectPtr<UUserWidget> CurrentWidget;

protected:
    //----------------------
    //  GameInProgress
    //----------------------

    UPROPERTY(BlueprintReadOnly, Category = "UI|GameInProgress")
    TObjectPtr<UGameplayWidget> GameplayWidget;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI|GameInProgress")
    TSubclassOf<UGameplayWidget> GameplayWidgetClass;

    UFUNCTION(BlueprintCallable, Category = "UI|GameInProgress")
    void OpenGameplayWidget();

public:
    //----------------------
    //  GameInPause
    //----------------------

    UFUNCTION(BlueprintCallable, Category = "UI|GameInPause")
    void OpenSettingsCollection();

protected:
    UPROPERTY(BlueprintReadOnly, Category = "UI|GameInPause")
    TObjectPtr<USettingsCollectionWidget> SettingsCollectionWidget;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI|GameInPause")
    TSubclassOf<USettingsCollectionWidget> SettingsCollectionWidgetClass;

    UPROPERTY(BlueprintReadOnly, Category = "UI|GameInPause")
    TObjectPtr<UPauseMenuWidget> PauseMenuWidget;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI|GameInPause")
    TSubclassOf<UPauseMenuWidget> PauseMenuWidgetClass;

private:
    UFUNCTION()
    void OnGamePauseChanged(const bool bIsPaused);

private:
    //----------------------
    //  GameOver
    //----------------------

    UFUNCTION()
    void OpenGameOverWidget();

protected:
    UPROPERTY(BlueprintReadOnly, Category = "UI|GameOver")
    TObjectPtr<UGameOverWidget> GameOverWidget;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI|GameOver")
    TSubclassOf<UGameOverWidget> GameOverWidgetClass;
};
