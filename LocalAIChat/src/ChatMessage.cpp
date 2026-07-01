#include "ChatMessage.h"

#include <utility>

FChatMessage::FChatMessage(EMessageRole InRole, std::string InText)
    : Role(InRole)
    , Text(std::move(InText))
{
}

EMessageRole FChatMessage::GetRole() const
{
    return Role;
}

const std::string& FChatMessage::GetText() const
{
    return Text;
}

std::string FChatMessage::GetRoleName() const
{
    switch (Role)
    {
    case EMessageRole::User:
        return "User";
    case EMessageRole::Assistant:
        return "AI";
    default:
        return "Unknown";
    }
}
