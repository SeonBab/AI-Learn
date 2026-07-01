#pragma once

#include <string>

class Item
{
public:
    Item() = default;
    Item(std::string id, std::string name, std::string description);

    const std::string& getId() const;
    const std::string& getName() const;
    const std::string& getDescription() const;

private:
    std::string id_;
    std::string name_;
    std::string description_;
};
