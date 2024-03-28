// A test task by KEFIR
#include "BaseSpacecraft.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/BoxComponent.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "Components/HealthComponent.h"
#include "Components/WeaponComponent.h"

ABaseSpacecraft::ABaseSpacecraft()
{
    PrimaryActorTick.bCanEverTick = false;

    BoxCollision = CreateDefaultSubobject<UBoxComponent>("BoxCollision");
    checkf(BoxCollision, TEXT("BoxCollision doesn't exist!"));
    BoxCollision->SetCollisionProfileName(UCollisionProfile::Pawn_ProfileName);
    BoxCollision->SetNotifyRigidBodyCollision(true);

    const auto BoxBodyInstance = BoxCollision->GetBodyInstance();
    if (BoxBodyInstance != nullptr)
    {
        BoxBodyInstance->bLockZTranslation = true;
    }

    SetRootComponent(BoxCollision);

    MeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>("MeshComponent");
    checkf(MeshComponent, TEXT("MeshComponent doesn't exist!"));
    MeshComponent->SetupAttachment(BoxCollision);

    MovementComponent = CreateDefaultSubobject<UFloatingPawnMovement>("MovementComponent");
    checkf(MovementComponent, TEXT("MovementComponent doesn't exist!"));

    HealthComponent = CreateDefaultSubobject<UHealthComponent>("HealthComponent");
    checkf(HealthComponent, TEXT("HealthComponent doesn't exist!"));

    WeaponComponent = CreateDefaultSubobject<UWeaponComponent>("WeaponComponent");
    checkf(WeaponComponent, TEXT("WeaponComponent doesn't exist!"));
}

void ABaseSpacecraft::BeginPlay()
{
    Super::BeginPlay();

    {
        checkf(LifeSpanOnDeath > 0.0f, TEXT("LifeSpanOnDeath must be more than zero!"));

        checkf(HitDamage > 0, TEXT("HitDamage must be more than zero!"));
    }

    OnActorHit.AddDynamic(this, &ThisClass::OnActorHitReceive);

    HealthComponent->OnDeath.AddDynamic(this, &ThisClass::OnDeath);
}

void ABaseSpacecraft::OnActorHitReceive(AActor* SelfActor, AActor* OtherActor, const FVector NormalImpulse, const FHitResult& Hit)
{
    if (!IsValid(OtherActor)) return;
    OtherActor->TakeDamage(HitDamage, FDamageEvent(), Controller, this);
}

void ABaseSpacecraft::OnDeath()
{
    BoxCollision->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
    SetLifeSpan(LifeSpanOnDeath);

    OnDeath_Visual();
}
void ABaseSpacecraft::OnDeath_Visual_Implementation() {}
