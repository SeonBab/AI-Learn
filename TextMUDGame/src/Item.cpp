#include "Item.h"

#include <utility>

Item::Item(std::string id, std::string name, std::string description)
    : id_(std::move(id)),
      name_(std::move(name)),
      description_(std::move(description))
{
}

const std::string& Item::getId() const
{
    return id_;
}

const std::string& Item::getName() const
{
    return name_;
}

const std::string& Item::getDescription() const
{
    return description_;
}
