// A test task by KEFIR
#include "Weapons/Ranged/Projectiles/ProjectileBase.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/BoxComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "AsteroidCoreTypes.h"

AProjectileBase::AProjectileBase()
{
    PrimaryActorTick.bCanEverTick = true;

    BoxCollision = CreateDefaultSubobject<UBoxComponent>("BoxCollision");
    checkf(BoxCollision, TEXT("BoxCollision doesn't exist!"));
    BoxCollision->SetCollisionProfileName(ProjectileCollisionProfileName);
    SetRootComponent(BoxCollision);

    MeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>("MeshComponent");
    checkf(MeshComponent, TEXT("MeshComponent doesn't exist!"));
    MeshComponent->SetupAttachment(BoxCollision);

    MovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>("MovementComponent");
    checkf(MovementComponent, TEXT("MovementComponent doesn't exist!"));
    MovementComponent->InitialSpeed = 1500.0f;
    MovementComponent->MaxSpeed = 3000.0f;
    MovementComponent->ProjectileGravityScale = 0.0f;
}

void AProjectileBase::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void AProjectileBase::BeginPlay()
{
    Super::BeginPlay();
    checkf(Damage > 0, TEXT("Damage must be more than zero!"));

    OnActorBeginOverlap.AddDynamic(this, &ThisClass::OnProjectileHitReceive);
}

void AProjectileBase::OnProjectileHitReceive(AActor* OverlappedActor, AActor* OtherActor)
{
    if (!OtherActor || OtherActor == GetOwner() || OtherActor == GetInstigator()) return;

    MakePointDamage(OtherActor);
    Destroy();
}

bool AProjectileBase::MakePointDamage(AActor* DamageTaker)
{
    if (!DamageTaker) return false;

    const APawn* MyInstigator = GetInstigator();
    if (!MyInstigator) return false;

    AController* EventInstigator = MyInstigator->GetController();
    FPointDamageEvent PointDamageEvent;

    DamageTaker->TakeDamage(Damage, PointDamageEvent, EventInstigator, GetOwner());

    return true;
}
