// A test task by KEFIR
#include "Settings/BaseGameSetting.h"

//-----------------------------
//  Base GameSetting
//-----------------------------

void UBaseGameSetting::SetName(const FText& InName)
{
    Name = InName;
}

FText UBaseGameSetting::GetName() const
{
    return Name;
}
