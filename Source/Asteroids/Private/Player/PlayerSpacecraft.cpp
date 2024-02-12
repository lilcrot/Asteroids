// A test task by KEFIR

#include "Player/PlayerSpacecraft.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/WeaponComponent.h"

APlayerSpacecraft::APlayerSpacecraft()
{
    PrimaryActorTick.bCanEverTick = true;
}

void APlayerSpacecraft::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    LookToMouse(DeltaTime);
}

void APlayerSpacecraft::BeginPlay()
{
    Super::BeginPlay();

    if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
    {
        auto InputSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());
        if (InputSubsystem)
        {
            InputSubsystem->AddMappingContext(DefaultContext, 1);
        }
    }
}

void APlayerSpacecraft::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);
    if (!EnhancedInputComponent) return;

    EnhancedInputComponent->BindAction(MovementAction, ETriggerEvent::Triggered, this, &ThisClass::Move);

    EnhancedInputComponent->BindAction(FirstWeaponFireAction, ETriggerEvent::Started, this, &ThisClass::FirstWeaponFire);
    EnhancedInputComponent->BindAction(FirstWeaponFireAction, ETriggerEvent::Completed, this, &ThisClass::StopFire);
}

void APlayerSpacecraft::Move(const FInputActionValue& Value)
{
    const FVector2D MovementVector = Value.Get<FVector2D>();

    AddMovementInput(FVector::ForwardVector, MovementVector.X);
    AddMovementInput(FVector::RightVector, MovementVector.Y);
}

void APlayerSpacecraft::FirstWeaponFire()
{
    if (!WeaponComponent) return;
    WeaponComponent->StartFireByIndex(0);
}

void APlayerSpacecraft::StopFire() 
{
    if (!WeaponComponent) return;
    WeaponComponent->StopFire();
}

void APlayerSpacecraft::LookToMouse(float DeltaTime)
{
    const auto* PlayerController = Cast<APlayerController>(Controller);
    if (!PlayerController) return;

    FHitResult Hit;
    PlayerController->GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, false, Hit);

    const FRotator TargetRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), Hit.Location);
    const FRotator RInterpResult = UKismetMathLibrary::RInterpTo(GetActorRotation(), TargetRotation, DeltaTime, RotationSpeed);

    SetActorRotation(FRotator(0.0, RInterpResult.Yaw, 0.0));
}
