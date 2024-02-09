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

    TryAttachToOwner();
}

void ABaseWeapon::StartFire() 
{
    bFiring = true;
}

void ABaseWeapon::StopFire() 
{
    bFiring = false;
}

bool ABaseWeapon::TryAttachToOwner()
{
    auto* MyOwner = GetOwner();
    if (!MyOwner) return false;

    FAttachmentTransformRules TransformRules = FAttachmentTransformRules::SnapToTargetIncludingScale;
    return AttachToActor(MyOwner, TransformRules, OwnerAttackSocketName);
}
