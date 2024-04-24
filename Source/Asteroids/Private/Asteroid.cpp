// A test task by KEFIR
#include "Asteroid.h"
#include "Components/SphereComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/HealthComponent.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "AsteroidCoreTypes.h"

AAsteroid::AAsteroid()
{
    PrimaryActorTick.bCanEverTick = true;

    {
        SphereCollision = CreateDefaultSubobject<USphereComponent>("SphereCollision");
        checkf(SphereCollision, TEXT("SphereCollision doesn't exist!"));
        SphereCollision->SetCollisionProfileName(AsteroidCollisionProfileName);
        SphereCollision->SetNotifyRigidBodyCollision(true);
        SphereCollision->SetSimulatePhysics(true);
        SphereCollision->SetEnableGravity(false);

        const auto SphereBodyInstance = SphereCollision->GetBodyInstance();
        if (SphereBodyInstance != nullptr)
        {
            SphereBodyInstance->bLockZTranslation = true;
        }

        SphereCollision->bApplyImpulseOnDamage = false;
        SphereCollision->SetAngularDamping(5.0f);
        SphereCollision->SetLinearDamping(1.5f);

        SetRootComponent(SphereCollision);
    }

    SkeletalMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>("SkeletalMeshComponent");
    checkf(SkeletalMeshComponent, TEXT("SkeletalMeshComponent doesn't exist!"));
    SkeletalMeshComponent->SetupAttachment(SphereCollision);

    HealthComponent = CreateDefaultSubobject<UHealthComponent>("HealthComponent");
    checkf(HealthComponent, TEXT("HealthComponent doesn't exist!"));

    MovementComponent = CreateDefaultSubobject<UFloatingPawnMovement>("MovementComponent");
    checkf(MovementComponent, TEXT("MovementComponent doesn't exist!"));

    AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}

void AAsteroid::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    AddMovementInput(GetActorForwardVector());

    SkeletalMeshComponent->AddLocalRotation(RotationSpeed);
}

FVector AAsteroid::GetVelocity() const
{
    return MovementComponent->Velocity;
}

void AAsteroid::BeginPlay()
{
    Super::BeginPlay();

    {
        checkf(MaxAcceleration > MinAcceleration, TEXT("MaxAcceleration must be more than MinAcceleration!"));
        MovementComponent->Acceleration = FMath::RandRange(MaxAcceleration, MinAcceleration);

        checkf(MaxSpeed > 0.0f, TEXT("MaxSpeed must be more than zero!"));
        MovementComponent->MaxSpeed = MaxSpeed;

        checkf(HitDamage > 0, TEXT("HitDamage must be more than zero!"));

        checkf(LifeSpanOnDeath > 0.0f, TEXT("LifeSpanOnDeath must be more than zero!"));
    }
    HealthComponent->OnDeath.AddDynamic(this, &ThisClass::OnDeath);

    OnActorHit.AddDynamic(this, &ThisClass::OnActorHitReceive);
}

void AAsteroid::OnDeath_Implementation()
{
    SphereCollision->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
    SetLifeSpan(LifeSpanOnDeath);
}

void AAsteroid::OnActorHitReceive(AActor* SelfActor, AActor* OtherActor, const FVector NormalImpulse, const FHitResult& Hit)
{
    if (!IsValid(OtherActor)) return;
    OtherActor->TakeDamage(HitDamage, FDamageEvent(), Controller, this);

    FVector Impulse = Hit.ImpactNormal * (GetVelocity().Size() + OtherActor->GetVelocity().Size());
    Impulse.Z = 0.0;

    SphereCollision->AddImpulse(Impulse, NAME_None, true);
}
