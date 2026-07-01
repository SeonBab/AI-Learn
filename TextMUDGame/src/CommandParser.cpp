#include "CommandParser.h"

#include <sstream>

Command CommandParser::parse(const std::string& inputText) const
{
    Command command;
    command.originalText = inputText;

    std::vector<std::string> words = split(inputText);
    if (words.empty())
    {
        return command;
    }

    const std::string& action = words[0];
    const std::size_t actionStart = inputText.find(action);
    if (actionStart != std::string::npos)
    {
        const std::size_t targetStart = actionStart + action.size();
        command.target = trim(inputText.substr(targetStart));
    }

    if (action == "look")
    {
        command.type = CommandType::Look;
    }
    else if (action == "help")
    {
        command.type = CommandType::Help;
    }
    else if (action == "quit")
    {
        command.type = CommandType::Quit;
    }
    else if (action == "npc" || action == "npcs")
    {
        command.type = CommandType::Npc;
    }
    else if (action == "items")
    {
        command.type = CommandType::Items;
    }
    else if (action == "inventory")
    {
        command.type = CommandType::Inventory;
    }
    else if (action == "talk")
    {
        command.type = CommandType::Talk;
    }
    else if (action == "take")
    {
        command.type = CommandType::Take;
    }
    else if (action == "use")
    {
        command.type = CommandType::Use;
    }
    else if (action == "ask")
    {
        command.type = CommandType::Ask;
    }
    else if (action == "save")
    {
        command.type = CommandType::Save;
    }
    else if (action == "load")
    {
        command.type = CommandType::Load;
    }
    else if (action == "go" && words.size() >= 2 && isDirection(words[1]))
    {
        command.type = CommandType::Move;
        command.arguments.push_back(words[1]);
        return command;
    }
    else if (isDirection(action))
    {
        command.type = CommandType::Move;
        command.arguments.push_back(action);
        return command;
    }

    for (std::size_t i = 1; i < words.size(); ++i)
    {
        command.arguments.push_back(words[i]);
    }

    return command;
}

std::string CommandParser::trim(const std::string& text) const
{
    const std::size_t first = text.find_first_not_of(" \t\r\n");
    if (first == std::string::npos)
    {
        return "";
    }

    const std::size_t last = text.find_last_not_of(" \t\r\n");
    return text.substr(first, last - first + 1);
}

std::vector<std::string> CommandParser::split(const std::string& inputText) const
{
    std::istringstream stream(inputText);
    std::vector<std::string> words;
    std::string word;

    while (stream >> word)
    {
        words.push_back(word);
    }

    return words;
}

bool CommandParser::isDirection(const std::string& word) const
{
    return word == "north" || word == "south" || word == "east" || word == "west";
}
