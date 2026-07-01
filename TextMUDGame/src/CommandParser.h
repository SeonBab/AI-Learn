#pragma once

#include <string>
#include <vector>

enum class CommandType
{
    Look,
    Move,
    Npc,
    Items,
    Take,
    Use,
    Inventory,
    Talk,
    Ask,
    Save,
    Load,
    Quit,
    Help,
    Unknown
};

struct Command
{
    CommandType type = CommandType::Unknown;
    std::vector<std::string> arguments;
    std::string target;
    std::string originalText;
};

class CommandParser
{
public:
    Command parse(const std::string& inputText) const;

private:
    std::string trim(const std::string& text) const;
    std::vector<std::string> split(const std::string& inputText) const;
    bool isDirection(const std::string& word) const;
};
