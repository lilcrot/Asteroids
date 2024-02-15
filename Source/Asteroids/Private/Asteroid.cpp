// A test task by KEFIR
#include "Asteroid.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/SphereComponent.h"
#include "Components/HealthComponent.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "AsteroidCoreTypes.h"
#include "WorldBoundary.h"

AAsteroid::AAsteroid()
{
    PrimaryActorTick.bCanEverTick = true;

    SphereCollision = CreateDefaultSubobject<USphereComponent>("SphereCollision");
    checkf(SphereCollision, TEXT("SphereCollision doesn't exist!"));
    SphereCollision->SetCollisionProfileName(AsteroidCollisionProfileName);
    SetRootComponent(SphereCollision);

    MeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>("MeshComponent");
    checkf(MeshComponent, TEXT("MeshComponent doesn't exist!"));
    MeshComponent->SetupAttachment(SphereCollision);

    HealthComponent = CreateDefaultSubobject<UHealthComponent>("HealthComponent");
    checkf(HealthComponent, TEXT("HealthComponent doesn't exist!"));

    MovementComponent = CreateDefaultSubobject<UFloatingPawnMovement>("MovementComponent");
    checkf(MovementComponent, TEXT("MovementComponent doesn't exist!"));

    AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}

void AAsteroid::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    AddMovementInput(GetActorForwardVector(), CurrentVelocityScaleValue);
}

void AAsteroid::BeginPlay()
{
    Super::BeginPlay();

    {
        checkf(MaxAcceleration > MinAcceleration, TEXT("MaxAcceleration must be more than MinAcceleration!"));
        MovementComponent->Acceleration = FMath::RandRange(MaxAcceleration, MinAcceleration);

        checkf(MaxSpeed > 0.0f, TEXT("MaxSpeed must be more than zero!"));
        MovementComponent->MaxSpeed = MaxSpeed;

        checkf(OverlapDamage > 0.0f, TEXT("OverlapDamage must be more than zero!"));

        checkf(VelocityDivideCoefficient > 0.0f, TEXT("VelocityDivideCoefficient must be more than zero!"));
    }

    OnActorBeginOverlap.AddDynamic(this, &ThisClass::OnActorBeginOverlapReceive);

    HealthComponent->OnDeath.AddDynamic(this, &ThisClass::OnDeath);
}

void AAsteroid::OnDeath_Implementation()
{
    SphereCollision->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
    Destroy();
}

void AAsteroid::OnActorBeginOverlapReceive(AActor* OverlappedActor, AActor* OtherActor)
{
    if (!OtherActor || OtherActor->IsA(AWorldBoundary::StaticClass())) return;

    /* Asteroids only attack another actors, if an asteroid is hit by another asteroid, then they should only change their direction */
    if (!OtherActor->IsA(AAsteroid::StaticClass()))
    {
        OtherActor->TakeDamage(OverlapDamage, FDamageEvent(), Controller, this);
    }

    CurrentVelocityScaleValue /= VelocityDivideCoefficient;
    StartRecoverVelocity();
}

void AAsteroid::StartRecoverVelocity()
{
    UWorld* World = GetWorld();
    if (!World) return;

    constexpr float TimerRate = 0.01f;
    World->GetTimerManager().SetTimer(
        RecoverVelocityTimerHandle,
        [&]()
        {
            CurrentVelocityScaleValue = FMath::Clamp(CurrentVelocityScaleValue + 0.01f, 0.0f, 1.0f);

            if (CurrentVelocityScaleValue >= 1.0f && GetWorld()) GetWorld()->GetTimerManager().ClearTimer(RecoverVelocityTimerHandle);
        },
        TimerRate, true, -1.0f);
}