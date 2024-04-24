// A test task by KEFIR
#pragma once
#include "Tests/InputRecordingTypes.h"
#include "CoreMinimal.h"

struct FInputActionInstance;

namespace Test
{
class JsonUtils
{
public:
    static bool WriteInputData(const FString& FileName, const FInputData& InputData);
    static bool ReadInputData(const FString& FileName, FInputData& InputData);
};

}  // namespace Test
