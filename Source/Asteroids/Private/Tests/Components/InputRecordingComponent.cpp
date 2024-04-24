// A test task by KEFIR
#include "Tests/Components/InputRecordingComponent.h"
#include "Engine/World.h"
#include "EnhancedInputComponent.h"
#include "Tests/Utils/JsonUtils.h"
#include "EnhancedPlayerInput.h"

using namespace Test;

UInputRecordingComponent::UInputRecordingComponent()
{
    PrimaryComponentTick.bCanEverTick = true;
}

void UInputRecordingComponent::BeginPlay()
{
    Super::BeginPlay();

    const auto* Pawn = Cast<APawn>(GetOwner());
    checkf(Pawn, TEXT("Owner isn't pawn, this component implied input component"));

    checkf(GetWorld(), TEXT("World is null"));
    checkf(Pawn->InputComponent, TEXT("Owner InputComponent is null"));

    const auto* PlayerController = Pawn->GetController<APlayerController>();
    checkf(PlayerController, TEXT("Owner PlayerController is null, this component implied it"));

    EnhancedPlayerInput = Cast<UEnhancedPlayerInput>(PlayerController->PlayerInput);
    checkf(EnhancedPlayerInput, TEXT("EnhancedPlayerInput is null, this component serialize only enhanced inputs!"));

    InputData.InitialTransform = GetOwner()->GetActorTransform();
    InputData.Bindings.Add(MakeBindingsData());
}

void UInputRecordingComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    Super::EndPlay(EndPlayReason);
    JsonUtils::WriteInputData(GenerateFileName(), InputData);
}

void UInputRecordingComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
    InputData.Bindings.Add(MakeBindingsData());
}

FBindingsData UInputRecordingComponent::MakeBindingsData() const
{
    UWorld* World = GetWorld();
    if (!EnhancedPlayerInput || !World) return FBindingsData();

    APawn* MyPawn = Cast<APawn>(GetOwner());
    if (!MyPawn) return FBindingsData();

    UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(MyPawn->InputComponent);
    if (!EnhancedInputComponent) return FBindingsData();

    FBindingsData BindingsData;
    BindingsData.WorldTime = World->TimeSeconds;

    for (const TUniquePtr<FEnhancedInputActionEventBinding>& ActionBinding : EnhancedInputComponent->GetActionEventBindings())
    {
        const UInputAction* Action = ActionBinding->GetAction();
        if (const FInputActionInstance* ActionData = EnhancedPlayerInput->FindActionInstanceData(Action))
        {
            BindingsData.ActionsData.Add(FActionData{ActionData->GetSourceAction()->GetFName(),  //
                ActionData->GetValue().Get<FVector>(), ActionData->GetTriggerEvent()});          //
        }
    }

    return BindingsData;
}

FString UInputRecordingComponent::GenerateFileName() const
{
    FString SavedDir = FPaths::ProjectSavedDir();
    const FString Date = FDateTime::Now().ToString();
    return SavedDir.Append("/Tests/").Append(FileName).Append("_").Append(Date).Append(".json");
}