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

    InitialLifeSpan = 5.0f;
}

void AProjectileBase::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void AProjectileBase::BeginPlay()
{
    Super::BeginPlay();
    checkf(Damage > 0, TEXT("Damage must be more than zero!"));

    OnActorBeginOverlap.AddDynamic(this, &ThisClass::OnProjectileOverlapReceive);
}

void AProjectileBase::OnProjectileOverlapReceive(AActor* OverlappedActor, AActor* OtherActor)
{
    if (!IsValid(OtherActor) || OtherActor == GetOwner() || OtherActor == GetInstigator()) return;

    MakePointDamage(OtherActor);
    Destroy();
}

bool AProjectileBase::MakePointDamage(AActor* DamageTaker)
{
    if (!IsValid(DamageTaker)) return false;

    AController* EventInstigator = nullptr;

    const APawn* MyInstigator = GetInstigator();
    if (IsValid(MyInstigator))
    {
        EventInstigator = MyInstigator->GetController();
    }

    FPointDamageEvent PointDamageEvent;
    DamageTaker->TakeDamage(Damage, PointDamageEvent, EventInstigator, GetOwner());

    return true;
}
