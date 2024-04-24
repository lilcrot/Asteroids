// A test task by KEFIR

#if WITH_AUTOMATION_TESTS

#include "Tests/GameplayTests.h"
#include "CoreMinimal.h"
#include "Misc/AutomationTest.h"
#include "Tests/Utils/TestUtils.h"
#include "Tests/TestConstants.h"
#include "Components/HealthComponent.h"

using namespace Test;
using namespace TestConstants;

namespace
{
struct FActorTestInfo
{
    AActor* GetActor() const { return Actor; }
    UHealthComponent* GetHealthComponent() const { return HealthComponent; }
    int32 GetInitHealth() const { return InitHealth; }

    void SetActor(AActor* InActor)
    {
        Actor = InActor;
        if (!Actor) return;

        HealthComponent = Actor->FindComponentByClass<UHealthComponent>();
        if (HealthComponent)
        {
            InitHealth = HealthComponent->GetHealth();
        }
    }

private:
    TObjectPtr<AActor> Actor;

    TObjectPtr<UHealthComponent> HealthComponent;
    int32 InitHealth = 0;
};

FActorTestInfo FirstActorInfo, SecondActorInfo, TargetToProjectileActorInfo;
}  // namespace

IMPLEMENT_COMPLEX_AUTOMATION_TEST(FCanDamageEachOther, "Asteroids.Gameplay.CanDamageEachOther",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::CriticalPriority);

void FCanDamageEachOther::GetTests(TArray<FString>& OutBeautifiedNames, TArray<FString>& OutTestCommands) const
{
    struct FTestData
    {
        FString TestName;
        FString MapName;
        FString FirstBPName;
        FString SecondBPName;
    };

    const TArray<FTestData> TestData =  //
        {
            {"Asteroid_Asteroid", TestEmptyMapName, BigAsteroidBPName, SmallAsteroidBPName},         //
            {"Player_Asteroid", TestEmptyMapName, PlayerSpacecraftTestBPName, SmallAsteroidBPName},  //
            {"UFO_Asteroid", TestEmptyMapName, UFOEnemyTestBPName, SmallAsteroidBPName}              //
        };

    for (const auto OneTestData : TestData)
    {
        OutBeautifiedNames.Add(OneTestData.TestName);
        OutTestCommands.Add(FString::Printf(TEXT("%s,%s,%s"), *OneTestData.MapName, *OneTestData.FirstBPName, *OneTestData.SecondBPName));
    }
}
bool FCanDamageEachOther::RunTest(const FString& Parameters)
{
    TArray<FString> ParsedParams;
    Parameters.ParseIntoArray(ParsedParams, TEXT(","));
    if (!TestTrue("MapName and BPNames params should exist", ParsedParams.Num() == 3)) return false;

    AutomationOpenMap(ParsedParams[0]);
    UWorld* World = GetTestGameWorld();
    if (!TestNotNull("World is opened", World)) return false;

    const FVector FirstSpawnLocation = FVector(50, 0.0, 0.0);
    const FRotator FirstSpawnRotation = FRotator(0.0, 180, 0.0);

    {
        FirstActorInfo.SetActor(CreateBlueprint<AActor>(World, ParsedParams[1],  //
            FTransform(FirstSpawnRotation, FirstSpawnLocation)));                //

        if (!TestTrue("FirstActor is spawned", IsValid(FirstActorInfo.GetActor()))) return false;
        if (!TestTrue("The FirstActor's HealthComponent exists", IsValid(FirstActorInfo.GetHealthComponent()))) return false;
    }

    {
        SecondActorInfo.SetActor(CreateBlueprint<AActor>(World, ParsedParams[2],                      //
            FTransform(FirstSpawnRotation - FRotator(0.0, 180.0, 0.0), FirstSpawnLocation * -1.0)));  //
        if (!TestTrue("SecondActor is spawned", IsValid(SecondActorInfo.GetActor()))) return false;
        if (!TestTrue("The SecondActor's HealthComponent exists", IsValid(SecondActorInfo.GetHealthComponent()))) return false;
    }

    ADD_LATENT_AUTOMATION_COMMAND(FEngineWaitLatentCommand(0.4))
    ADD_LATENT_AUTOMATION_COMMAND(FFunctionLatentCommand(
        [&]()
        {
            const auto* FirstHealthComponent = FirstActorInfo.GetHealthComponent();
            if (IsValid(FirstHealthComponent))
            {
                TestTrue("Damage for fist actor was taken", FirstActorInfo.GetInitHealth() > FirstHealthComponent->GetHealth());
            }
            else
            {
                TestTrue("If FirstHealthComponent isn't valid then FirstActor must be also invalid", !IsValid(FirstActorInfo.GetActor()));
            }

            const auto* SecondHealthComponent = SecondActorInfo.GetHealthComponent();
            if (IsValid(SecondHealthComponent))
            {
                TestTrue("Damage for second actor was taken", SecondActorInfo.GetInitHealth() > SecondHealthComponent->GetHealth());
            }
            else
            {
                TestTrue(
                    "If SecondHealthComponent isn't valid then SecondActor must be also invalid", !IsValid(SecondActorInfo.GetActor()));
            }

            return true;
        }));

    ADD_LATENT_AUTOMATION_COMMAND(FExitGameCommand);
    return true;
}

/*------------------------------------------------*/
IMPLEMENT_COMPLEX_AUTOMATION_TEST(FProjectileCanDamage, "Asteroids.Gameplay.ProjectileCanDamage",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::CriticalPriority);

void FProjectileCanDamage::GetTests(TArray<FString>& OutBeautifiedNames, TArray<FString>& OutTestCommands) const
{
    struct FTestData
    {
        FString TestName;
        FString TargetBPName;
    };

    const TArray<FTestData> TestData =  //
        {
            {"BigAsteroid", BigAsteroidBPName},      //
            {"SmallAsteroid", SmallAsteroidBPName},  //
            {"Player", PlayerSpacecraftTestBPName},  //
            {"UFO", UFOEnemyTestBPName}              //
        };

    for (const auto OneTestData : TestData)
    {
        OutBeautifiedNames.Add(OneTestData.TestName);
        OutTestCommands.Add(OneTestData.TargetBPName);
    }
}
bool FProjectileCanDamage::RunTest(const FString& Parameters)
{
    AutomationOpenMap(TestEmptyMapName);
    UWorld* World = GetTestGameWorld();
    if (!TestNotNull("World is opened", World)) return false;

    const FVector FirstSpawnLocation = FVector(150, 0.0, 0.0);
    const FRotator FirstSpawnRotation = FRotator(0.0, 180, 0.0);

    {
        TargetToProjectileActorInfo.SetActor(CreateBlueprint<AActor>(World, Parameters,  //
            FTransform(FirstSpawnRotation, FirstSpawnLocation)));                        //
        if (!TestTrue("TargetToProjectileActor is spawned", IsValid(TargetToProjectileActorInfo.GetActor()))) return false;
        if (!TestTrue("TargetToProjectile's HealthComponent exists", IsValid(TargetToProjectileActorInfo.GetHealthComponent())))
            return false;
    }

    {
        const auto* Projectile = CreateBlueprint<AActor>(World, ProjectileBPName,                    //
            FTransform(FirstSpawnRotation - FRotator(0.0, 180.0, 0.0), FirstSpawnLocation * -1.0));  //
        if (!TestNotNull("Projectile is spawned", Projectile)) return false;
    }

    ADD_LATENT_AUTOMATION_COMMAND(FEngineWaitLatentCommand(0.2))
    ADD_LATENT_AUTOMATION_COMMAND(FFunctionLatentCommand(
        [&]()
        {
            const auto* TargetToProjectileHealthComponent = TargetToProjectileActorInfo.GetHealthComponent();
            if (IsValid(TargetToProjectileHealthComponent))
            {
                TestTrue("Damage for TargetToProjectileActor was taken",
                    TargetToProjectileActorInfo.GetInitHealth() > TargetToProjectileHealthComponent->GetHealth());
            }
            else
            {
                TestTrue("If TargetToProjectileHealthComponent isn't valid then TargetToProjectileActor must be also invalid",
                    !IsValid(TargetToProjectileActorInfo.GetActor()));
            }

            return true;
        }));

    ADD_LATENT_AUTOMATION_COMMAND(FExitGameCommand);
    return true;
}

/*------------------------------------------------*/
IMPLEMENT_COMPLEX_AUTOMATION_TEST(FEntitiesCanDie, "Asteroids.Gameplay.EntitiesCanDie",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::CriticalPriority);

void FEntitiesCanDie::GetTests(TArray<FString>& OutBeautifiedNames, TArray<FString>& OutTestCommands) const
{
    struct FTestData
    {
        FString TestName;
        FString TargetBPName;
    };

    const TArray<FTestData> TestData =  //
        {
            {"BigAsteroid", BigAsteroidBPName},      //
            {"SmallAsteroid", SmallAsteroidBPName},  //
            {"Player", PlayerSpacecraftTestBPName},  //
            {"UFO", UFOEnemyTestBPName}              //
        };

    for (const auto OneTestData : TestData)
    {
        OutBeautifiedNames.Add(OneTestData.TestName);
        OutTestCommands.Add(OneTestData.TargetBPName);
    }
}
bool FEntitiesCanDie::RunTest(const FString& Parameters)
{
    AutomationOpenMap(TestEmptyMapName);
    UWorld* World = GetTestGameWorld();
    if (!TestNotNull("World is opened", World)) return false;

    FirstActorInfo.SetActor(CreateBlueprint<AActor>(World, Parameters));
    if (!TestTrue("Entity is spawned", IsValid(FirstActorInfo.GetActor()))) return false;
    if (!TestTrue("The entity's HealthComponent exists", IsValid(FirstActorInfo.GetHealthComponent()))) return false;

    FirstActorInfo.GetHealthComponent()->SetHealth(0);  // death

    ADD_LATENT_AUTOMATION_COMMAND(FEngineWaitLatentCommand(0.2))
    ADD_LATENT_AUTOMATION_COMMAND(FFunctionLatentCommand(
        [&]()
        {
            TestTrue("Entity is died", !IsValid(FirstActorInfo.GetActor()));
            TestTrue("Entity's HealthComponent is destroyed", !IsValid(FirstActorInfo.GetHealthComponent()));

            return true;
        }));

    ADD_LATENT_AUTOMATION_COMMAND(FExitGameCommand);
    return true;
}
#endif
