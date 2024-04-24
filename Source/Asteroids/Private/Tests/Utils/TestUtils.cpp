// A test task by KEFIR
#if WITH_AUTOMATION_TESTS

#include "Tests/Utils/TestUtils.h"
#include "Misc/OutputDeviceNull.h"
#include "AutomationBlueprintFunctionLibrary.h"
#include "BufferVisualizationData.h"

namespace Test
{
UWorld* GetTestGameWorld()
{
    const TIndirectArray<FWorldContext>& WorldContexts = GEngine->GetWorldContexts();
    for (const FWorldContext& Context : WorldContexts)
    {
        if ((Context.WorldType == EWorldType::PIE || Context.WorldType == EWorldType::Game) && Context.World())
        {
            return Context.World();
        }
    }

    return nullptr;
}

void CallFuncByNameWithParams(UObject* Object, const FString& FuncName, const TArray<FString>& Params)
{
    if (!Object) return;

    // Command pattern: "FunctionName Param1 Param2 Param3"
    FString Command = FuncName;
    for (const auto Param : Params)
    {
        Command.Append(" ").Append(Param);
    }
    FOutputDeviceNull OutputDeviceNull;
    Object->CallFunctionByNameWithArguments(*Command, OutputDeviceNull, nullptr, true);
}

FUntilLatentCommand::FUntilLatentCommand(TFunction<void()> InCallback, TFunction<void()> InTimeoutCallback, float InTimeout)
    : Callback(MoveTemp(InCallback)), TimeoutCallback(MoveTemp(InTimeoutCallback)), Timeout(InTimeout)
{
}

bool FUntilLatentCommand::Update()
{
    const double NewTime = FPlatformTime::Seconds();
    if (NewTime - StartTime >= Timeout)
    {
        TimeoutCallback();
        return true;
    }

    Callback();
    return false;
}

FString GetTestDataDir()
{
    return FPaths::GameSourceDir().Append("Asteroids/Public/Tests/Data/");
}

UWidget* FindWidgetByName(const UUserWidget* Widget, const FName& WidgetName)
{
    if (!Widget || !Widget->WidgetTree) return nullptr;

    UWidget* FoundWidget = nullptr;
    UWidgetTree::ForWidgetAndChildren(Widget->WidgetTree->RootWidget,
        [&](UWidget* Child)
        {
            if (Child && Child->GetFName().IsEqual(WidgetName))
            {
                FoundWidget = Child;
            }
        });
    return FoundWidget;
}

FTakeScreenshotLatentCommand::FTakeScreenshotLatentCommand(const FString& InScreenshotName) : ScreenshotName(InScreenshotName)
{
    FAutomationTestFramework::Get().OnScreenshotTakenAndCompared.AddRaw(this, &FTakeScreenshotLatentCommand::OnScreenshotTakenAndCompared);
}

FTakeScreenshotLatentCommand::~FTakeScreenshotLatentCommand()
{
    FAutomationTestFramework::Get().OnScreenshotTakenAndCompared.RemoveAll(this);
}

void FTakeScreenshotLatentCommand::OnScreenshotTakenAndCompared()
{
    CommandCompleted = true;
}

FTakeGameScreenshotLatentCommand::FTakeGameScreenshotLatentCommand(const FString& InScreenshotName)
    : FTakeScreenshotLatentCommand(InScreenshotName)
{
}

bool FTakeGameScreenshotLatentCommand::Update()
{
    if (!ScreenshotRequested)
    {
        const auto Options = UAutomationBlueprintFunctionLibrary::GetDefaultScreenshotOptionsForRendering();
        UAutomationBlueprintFunctionLibrary::TakeAutomationScreenshotInternal(GetTestGameWorld(), ScreenshotName, FString{}, Options);
        ScreenshotRequested = true;
    }
    return CommandCompleted;
}

FTakeUIScreenshotLatentCommand::FTakeUIScreenshotLatentCommand(const FString& InScreenshotName)
    : FTakeScreenshotLatentCommand(InScreenshotName)
{
}

bool FTakeUIScreenshotLatentCommand::Update()
{
    if (!ScreenshotSetupDone)
    {
        ScreenshotSetupDone = true;
        SetBufferVisualization("Opacity");
        return false;
    }

    if (!ScreenshotRequested)
    {
        const auto Options = UAutomationBlueprintFunctionLibrary::GetDefaultScreenshotOptionsForRendering();
        UAutomationBlueprintFunctionLibrary::TakeAutomationScreenshotOfUI_Immediate(GetTestGameWorld(), ScreenshotName, Options);
        ScreenshotRequested = true;
    }
    return CommandCompleted;
}

void FTakeUIScreenshotLatentCommand::OnScreenshotTakenAndCompared()
{
    FTakeScreenshotLatentCommand::OnScreenshotTakenAndCompared();
    SetBufferVisualization(NAME_None);
}

void FTakeUIScreenshotLatentCommand::SetBufferVisualization(const FName& VisualizeBuffer)
{
    if (UGameViewportClient* ViewportClient = GEngine->GameViewport)
    {
        static IConsoleVariable* ICVar =
            IConsoleManager::Get().FindConsoleVariable(FBufferVisualizationData::GetVisualizationTargetConsoleCommandName());
        if (ICVar)
        {
            if (ViewportClient->GetEngineShowFlags())
            {
                ViewportClient->GetEngineShowFlags()->SetVisualizeBuffer(VisualizeBuffer == NAME_None ? false : true);
                ViewportClient->GetEngineShowFlags()->SetTonemapper(VisualizeBuffer == NAME_None ? true : false);
                ICVar->Set(VisualizeBuffer == NAME_None ? TEXT("") : *VisualizeBuffer.ToString());
            }
        }
    }
}

FSimulateInputActionsLatentCommand::FSimulateInputActionsLatentCommand(UWorld* InWorld, UEnhancedInputComponent* InInputComponent,
    UEnhancedPlayerInput* InPlayerInput, const TArray<FBindingsData>& InBindingsData)
    : World(InWorld), EnhancedInputComponent(InInputComponent), BindingsData(InBindingsData), PlayerInput(InPlayerInput)
{
    checkf(World, TEXT("SimulateInputActionsLatentCommand: World is nullptr"));
    checkf(EnhancedInputComponent, TEXT("SimulateInputActionsLatentCommand: EnhancedInputComponent is nullptr"));
    checkf(PlayerInput, TEXT("SimulateInputActionsLatentCommand: EnhancedPlayerInput is nullptr"));
}

bool FSimulateInputActionsLatentCommand::Update()
{
    if (!World || !EnhancedInputComponent || !PlayerInput) return true;

    if (WorldStartTime == 0.0f)
    {
        WorldStartTime = World->TimeSeconds;
    }

    TArray<TUniquePtr<FEnhancedInputActionEventBinding>> TriggeredDelegates;
    while (World->TimeSeconds - WorldStartTime >= BindingsData[Index].WorldTime)
    {
        for (const TUniquePtr<FEnhancedInputActionEventBinding>& Binding : EnhancedInputComponent->GetActionEventBindings())
        {
            const ETriggerEvent BoundTriggerEvent = Binding->GetTriggerEvent();
            TMap<FName, FActionData> ActionDataMap;

            for (int32 j = 0; j < BindingsData[Index].ActionsData.Num(); ++j)
            {
                if (GetActionName(Binding->GetAction()) != BindingsData[Index].ActionsData[j].ActionName) continue;

                const ETriggerEvent CurrentTrigger = BindingsData[Index].ActionsData[j].CurrentTrigger;

                if (BoundTriggerEvent == CurrentTrigger ||
                    (BoundTriggerEvent == ETriggerEvent::Started &&
                        CurrentTrigger == ETriggerEvent::Triggered))  // InputRecording doesn't serialize TriggerEvent::Started so it's
                                                                      // our solvution

                {
                    // Record intent to trigger started as well as triggered
                    // EmplaceAt 0 for the "Started" event it is always guaranteed to fire before Triggered
                    if (BoundTriggerEvent == ETriggerEvent::Started)
                    {
                        TriggeredDelegates.EmplaceAt(0, Binding->Clone());
                    }
                    else
                    {
                        TriggeredDelegates.Emplace(Binding->Clone());
                    }
                    ActionDataMap.Emplace(GetActionName(Binding->GetAction()), BindingsData[Index].ActionsData[j]);
                }
            }
            for (TUniquePtr<FEnhancedInputActionEventBinding>& Delegate : TriggeredDelegates)
            {
                TObjectPtr<const UInputAction> DelegateAction = Delegate->GetAction();
                if (ActionDataMap.Contains(GetActionName(DelegateAction)))
                {
                    PlayerInput->InjectInputForAction(DelegateAction,                            //
                        FInputActionValue(ActionDataMap[GetActionName(DelegateAction)].Value));  //
                }
            }

            TriggeredDelegates.Reset();
            ActionDataMap.Reset();
        }
        if (++Index >= BindingsData.Num()) return true;
    }

    return false;
}

void SpecCloseLevel(UWorld* World)
{
    if (APlayerController* PC = World->GetFirstPlayerController())
    {
        PC->ConsoleCommand(TEXT("Exit"), true);
    }
}

}  // namespace Test

#endif