// A test task by KEFIR
#include "Player/PlayerSpacecraft.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/WeaponComponent.h"
#include "Player/SpacecraftPlayerController.h"
#include "Components/BoxComponent.h"
#include "AsteroidCoreTypes.h"

APlayerSpacecraft::APlayerSpacecraft()
{
    PrimaryActorTick.bCanEverTick = true;

    BoxCollision->SetCollisionProfileName(PlayerCollisionProfileName);
}

void APlayerSpacecraft::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    LookToMouse(DeltaTime);
}

void APlayerSpacecraft::BeginPlay()
{
    Super::BeginPlay();

    auto* PlayerController = Cast<APlayerController>(Controller);
    if (PlayerController == nullptr) return;

    auto* InputSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());
    if (InputSubsystem != nullptr)
    {
        InputSubsystem->AddMappingContext(DefaultContext, 1);
    }
}

void APlayerSpacecraft::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);

    EnhancedInputComponent->BindAction(MovementAction, ETriggerEvent::Triggered, this, &ThisClass::Move);

    EnhancedInputComponent->BindAction(FirstWeaponFireAction, ETriggerEvent::Started, this, &ThisClass::FirstWeaponFire);
    EnhancedInputComponent->BindAction(FirstWeaponFireAction, ETriggerEvent::Completed, this, &ThisClass::StopFire);

    EnhancedInputComponent->BindAction(SecondWeaponFireAction, ETriggerEvent::Started, this, &ThisClass::SecondWeaponFire);
    EnhancedInputComponent->BindAction(SecondWeaponFireAction, ETriggerEvent::Completed, this, &ThisClass::StopFire);

    EnhancedInputComponent->BindAction(ToggleGamePauseAction, ETriggerEvent::Completed, this, &ThisClass::ToggleGamePause);
}

void APlayerSpacecraft::Move(const FInputActionValue& Value)
{
    const FVector2D MovementVector = Value.Get<FVector2D>();

    AddMovementInput(FVector::ForwardVector, MovementVector.X);
    AddMovementInput(FVector::RightVector, MovementVector.Y);
}

void APlayerSpacecraft::FirstWeaponFire()
{
    WeaponComponent->StartFireByIndex(0);
}

void APlayerSpacecraft::SecondWeaponFire()
{
    WeaponComponent->StartFireByIndex(1);
}

void APlayerSpacecraft::StopFire()
{
    WeaponComponent->StopFire();
}

void APlayerSpacecraft::LookToMouse(float DeltaTime)
{
    const auto* PlayerController = Cast<APlayerController>(Controller);
    if (PlayerController == nullptr) return;

    FHitResult Hit;
    PlayerController->GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, false, Hit);

    const FRotator TargetRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), Hit.Location);
    const FRotator RInterpResult = UKismetMathLibrary::RInterpTo(GetActorRotation(), TargetRotation, DeltaTime, RotationSpeed);

    SetActorRotation(FRotator(0.0, RInterpResult.Yaw, 0.0));
}

void APlayerSpacecraft::ToggleGamePause()
{
    auto* MyPlayerController = Cast<ASpacecraftPlayerController>(Controller);
    if (MyPlayerController == nullptr) return;

    MyPlayerController->ToggleGamePause();
}

void APlayerSpacecraft::OnDeath() 
{
    Super::OnDeath();

    DisableInput(Cast<APlayerController>(Controller));
}
