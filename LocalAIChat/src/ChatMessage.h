#pragma once

#include <string>

enum class EMessageRole
{
    User,
    Assistant
};

class FChatMessage
{
public:
    FChatMessage(EMessageRole InRole, std::string InText);

    EMessageRole GetRole() const;
    const std::string& GetText() const;
    std::string GetRoleName() const;

private:
    EMessageRole Role;
    std::string Text;
};
