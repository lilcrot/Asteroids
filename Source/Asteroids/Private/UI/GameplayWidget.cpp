// A test task by KEFIR
#include "UI/GameplayWidget.h"
#include "Components/TextBlock.h"
#include "UI/RemainingTimeProgressBar.h"
#include "EnemyWavesGameModeBase.h"
#include "Components/WeaponComponent.h"
#include "Weapons/Ranged/Laser.h"

void UGameplayWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();
    {
        checkf(WaveRemainingTimeProgressBar, TEXT("WaveRemainingTimeProgressBar doesn't exist"));
        checkf(WaveNumberText, TEXT("WaveNumberText doesn't exist"));
        checkf(LaserShotsNumberText, TEXT("LaserShotsNumberText doesn't exist"));
        checkf(LaserShotsReloadTimeProgressBar, TEXT("LaserShotsReloadTimeProgressBar doesn't exist"));
    }

    UWorld* World = GetWorld();
    if (!World) return;

    const auto* MyPlayer = GetOwningPlayerPawn();
    checkf(MyPlayer, TEXT("MyPlayer isn't valid"));

    if (auto* GameMode = Cast<AEnemyWavesGameModeBase>(World->GetAuthGameMode()))
    {
        GameMode->OnNewWaveHasStarted.AddDynamic(this, &ThisClass::OnNewWaveHasStarted);
        OnNewWaveHasStarted(GameMode->GetCurrentWaveInfo());
    }

    PlayerWeaponComponent = MakeWeakObjectPtr<UWeaponComponent>(MyPlayer->FindComponentByClass<UWeaponComponent>());
    checkf(PlayerWeaponComponent.IsValid(), TEXT("PlayerWeaponComponent isn't valid!"));
    PlayerWeaponComponent->OnAllWeaponsSpawned.AddUObject(this, &ThisClass::OnAllPlayerWeaponsSpawned);
}

void UGameplayWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
    Super::NativeTick(MyGeometry, InDeltaTime);
    WaveRemainingTimeProgressBar->Tick(InDeltaTime);
    LaserShotsReloadTimeProgressBar->Tick(InDeltaTime);
}

void UGameplayWidget::OnAllPlayerWeaponsSpawned()
{
    if (!PlayerWeaponComponent.IsValid()) return;

    const auto Laser = PlayerWeaponComponent->GetWeaponByClass<ALaser>();
    checkf(Laser, TEXT("Laser player weapon isn't valid or doesn't exists!"));
    Laser->OnLaserShotsChanged.AddUObject(this, &ThisClass::SetLaserShotsNumber);

    SetLaserShotsNumber(Laser->GetCurrentLaserShots());
}

void UGameplayWidget::OnNewWaveHasStarted(const FCurrentWaveInfo WaveInfo)
{
    WaveNumberText->SetText(FText::AsNumber(WaveInfo.WaveNumber));

    WaveRemainingTimeProgressBar->StartProgressBar(WaveInfo.EnemyWavePeriodTime);
}

void UGameplayWidget::SetLaserShotsNumber(const int32 NewLaserShots)
{
    LaserShotsNumberText->SetText(FText::AsNumber(NewLaserShots));

    if (NewLaserShots == 0 && PlayerWeaponComponent.IsValid())
    {
        const auto Laser = PlayerWeaponComponent->GetWeaponByClass<ALaser>();
        checkf(Laser, TEXT("Laser player weapon isn't valid or doesn't exists!"));

        LaserShotsReloadTimeProgressBar->SetVisibility(ESlateVisibility::Visible);
        LaserShotsReloadTimeProgressBar->StartProgressBar(Laser->GetReloadingTime());
    }
    else
    {
        LaserShotsReloadTimeProgressBar->SetVisibility(ESlateVisibility::Hidden);
    }
}
