// A test task by KEFIR
#include "Weapons/Ranged/BaseRangedWeapon.h"
#include "AsteroidCoreTypes.h"
#include "Weapons/Ranged/Projectiles/ProjectileBase.h"

ABaseRangedWeapon::ABaseRangedWeapon() {}

void ABaseRangedWeapon::BeginPlay()
{
    Super::BeginPlay();
}

void ABaseRangedWeapon::StartFire()
{
    Super::StartFire();

    checkf(ShootRate > 0.0f, TEXT("ShootRate must be more than zero, else fire will always fail!"));

    if (UWorld* World = GetWorld())
    {
        World->GetTimerManager().SetTimer(FireHandle, this, &ThisClass::MakeShot,  //
            ShootRate, bAutoFire, -1.0f);                                          //
    }
}

void ABaseRangedWeapon::StopFire()
{
    Super::StopFire();

    if (UWorld* World = GetWorld())
    {
        World->GetTimerManager().ClearTimer(FireHandle);
    }
}

void ABaseRangedWeapon::MakeShot()
{
    UWorld* World = GetWorld();
    if (!World || !CanMakeShot())
    {
        StopFire();
        return;
    }

    FTransform SpawnTransform;
    FActorSpawnParameters SpawnParams;
    GetSpawnProjectileData(SpawnTransform, SpawnParams);

    World->SpawnActor<AProjectileBase>(ProjectileClass, SpawnTransform, SpawnParams);
}

void ABaseRangedWeapon::GetSpawnProjectileData(FTransform& SpawnTransform, FActorSpawnParameters& SpawnParams)
{
    const FTransform MuzzleSocketTransform = GetMuzzleSocketTransform();
    const FVector SpawnLocation = MuzzleSocketTransform.GetLocation();
    const FVector ShotDirection = MuzzleSocketTransform.GetRotation().Vector();  // Get Normalized direction vector

    SpawnTransform = FTransform(ShotDirection.Rotation(), SpawnLocation, FVector::OneVector);

    SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
    SpawnParams.Owner = this;
    SpawnParams.Instigator = Cast<APawn>(GetOwner());
}

FTransform ABaseRangedWeapon::GetMuzzleSocketTransform() const
{
    const auto* MyOwner = GetOwner();
    if (!MyOwner) return FTransform();

    const auto* OwnerMesh = MyOwner->FindComponentByClass<USkeletalMeshComponent>();
    if (!OwnerMesh) return FTransform();

    return OwnerMesh->GetSocketTransform(OwnerMuzzleSocketName);
}
