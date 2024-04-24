// A test task by KEFIR

#if WITH_AUTOMATION_TESTS

#include "CoreMinimal.h"
#include "Misc/AutomationTest.h"
#include "Misc/PathViews.h"
#include "Components/HealthComponent.h"
#include "Tests/Components/TestWeaponComponent.h"
#include "Tests/Utils/TestUtils.h"
#include "Tests/TestConstants.h"
#include "Kismet/GameplayStatics.h"
#include "Weapons/BaseWeapon.h"
#include "Player/PlayerSpacecraft.h"

BEGIN_DEFINE_SPEC(FAsteroidsComponets, "Asteroids",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority)
UWorld* World;
UHealthComponent* HealthComponent;
UTestWeaponComponent* WeaponComponent;
END_DEFINE_SPEC(FAsteroidsComponets)

using namespace Test;
using namespace TestConstants;

void FAsteroidsComponets::Define()
{

    Describe("Components.HealthComponent",
        [this]()
        {
            BeforeEach([this]() { HealthComponent = NewObject<UHealthComponent>(); });
            It("CanBeCreated",
                [this]()
                {
                    TestTrue("HealthComponent can be created", IsValid(HealthComponent));
                    if (!HealthComponent) return;
                });
            It("MainFunctions",
                [this]()
                {
                    if (!HealthComponent) return;

                    const int32 MaxHealth = HealthComponent->GetMaxHealth();

                    const TArray<TestPayload<int32, int32>> TestData{{-5, 0}, {MaxHealth + 2, MaxHealth}, {0, 0}, {MaxHealth, MaxHealth}};
                    for (const auto& Data : TestData)
                    {
                        HealthComponent->SetHealth(Data.TestValue);
                        const FString What =
                            FString::Printf(TEXT("Test value for SetHealth: %i  |  Expected: %i"), Data.TestValue, Data.ExpectedValue);

                        TestTrue(What, HealthComponent->GetHealth() == Data.ExpectedValue);
                    }

                    HealthComponent->SetHealth(0);
                    TestTrueExpr(HealthComponent->IsDead());
                });
        });

    It("Components.WeaponComponent.CanBeCreated",
        [this]()
        {
            WeaponComponent = NewObject<UTestWeaponComponent>();
            TestTrue("WeaponComponent can be created", IsValid(WeaponComponent));
        });
    Describe("Components.WeaponComponent",
        [this]()
        {
            BeforeEach(
                [this]()
                {
                    AutomationOpenMap(TestEmptyMapName);
                    World = GetTestGameWorld();

                    const auto TestPlayer = CreateBlueprint<APlayerSpacecraft>(World, PlayerSpacecraftTestBPName);
                    checkf(TestPlayer, TEXT("TestPlayer is failed to create with PlayerSpacecraftTestBPName"));
                    WeaponComponent = TestPlayer->FindComponentByClass<UTestWeaponComponent>();
                });
            AfterEach([this]() { SpecCloseLevel(World); });

            It("WeaponsAreDestroyedAfterCompDestroy",
                [this]()
                {
                    TestTrue("WeaponComponent is finded in TestPlayer", IsValid(WeaponComponent));
                    if (!WeaponComponent) return;

                    const auto& WeaponClasses = WeaponComponent->GetWeaponClasses();
                    TestTrue("WeaponClasses are set", !WeaponClasses.IsEmpty());

                    TArray<AActor*> FoundActors;
                    UGameplayStatics::GetAllActorsOfClass(World, ABaseWeapon::StaticClass(), FoundActors);
                    TestTrue("All weapons from WeaponComponent have been spawned", FoundActors.Num() == WeaponClasses.Num());

                    WeaponComponent->DestroyComponent();

                    UGameplayStatics::GetAllActorsOfClass(World, ABaseWeapon::StaticClass(), FoundActors);
                    TestTrue("All weapons are destroyed after WeaponComponent is destroyed", FoundActors.IsEmpty());
                });
        });
}

#endif