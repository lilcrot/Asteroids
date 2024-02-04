// A test task by KEFIR
#include "Components/HealthComponent.h"

UHealthComponent::UHealthComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
    CurrentHealth = MaxHealth;
    checkf(MaxHealth > 0, TEXT("DefaultHealth must be more than zero"));
}

void UHealthComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UHealthComponent::BeginPlay()
{
    Super::BeginPlay();

    AActor* MyOwner = GetOwner();
    if (MyOwner)
    {
        MyOwner->OnTakeAnyDamage.AddDynamic(this, &ThisClass::OnTakeAnyDamage);
    }
}

void UHealthComponent::OnTakeAnyDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType,  //
    AController* InstigatedBy, AActor* DamageCauser)                                                       //
{
    if (Damage <= 0.0f) return;
    SetHealth(CurrentHealth - Damage);
}

void UHealthComponent::SetHealth(const int32 NewHealth)
{
    CurrentHealth = FMath::Clamp(NewHealth, 0, MaxHealth);
    if (CurrentHealth <= 0) OnDeath.Broadcast();
}
