// A test task by KEFIR
#include "Weapons/BaseWeapon.h"
#include "BaseSpacecraft.h"

ABaseWeapon::ABaseWeapon()
{
    PrimaryActorTick.bCanEverTick = false;
}

void ABaseWeapon::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void ABaseWeapon::BeginPlay()
{
    Super::BeginPlay();
    checkf(DamageAmount > 0, TEXT("DamageAmount must be more than zero!"));

    TryAttachToOwner();
}

void ABaseWeapon::StartFire() {}

void ABaseWeapon::StopFire() {}

bool ABaseWeapon::TryAttachToOwner()
{
    auto* MyOwner = GetOwner();
    if (!IsValid(MyOwner)) return false;

    FAttachmentTransformRules TransformRules = FAttachmentTransformRules::SnapToTargetIncludingScale;
    return AttachToActor(MyOwner, TransformRules);
}
