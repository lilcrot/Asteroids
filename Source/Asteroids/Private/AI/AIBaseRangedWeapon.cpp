// A test task by KEFIR
#include "AI/AIBaseRangedWeapon.h"
#include "AI/BaseSpacecraftEnemy.h"

DEFINE_LOG_CATEGORY_STATIC(LogAIRangedWeapon, All, All);

void AAIBaseRangedWeapon::GetSpawnProjectileData(FTransform& SpawnTransform, FActorSpawnParameters& SpawnParams)
{
    const auto AIOwner = Cast<ABaseSpacecraftEnemy>(GetOwner());
    if (!AIOwner)
    {
        UE_LOG(LogAIRangedWeapon, Error, TEXT("AIRangedWeapon doesn't have AI owner (that must be IsA ABaseSpacecraftEnemy)"));
        return;
    }

    const auto CurrentTarget = AIOwner->GetCurrentTarget();
    if (!CurrentTarget)
    {
        UE_LOG(LogAIRangedWeapon, Error, TEXT("Enemy doesn't have current target but Fire is executed"));
        return;
    }

    const FVector ShotDirection = (CurrentTarget->GetActorLocation() - AIOwner->GetActorLocation()).Rotation().Vector();

    const FTransform MuzzleSocketTransform = GetMuzzleSocketTransform();
    const FVector SpawnLocation = MuzzleSocketTransform.GetLocation();

    SpawnTransform = FTransform(ShotDirection.Rotation(), SpawnLocation, FVector::OneVector);

    SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
    SpawnParams.Owner = this;
    SpawnParams.Instigator = Cast<APawn>(GetOwner());
}
