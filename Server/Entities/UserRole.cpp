#include "UserRole.h"

UserRole::UserRole(int id, const std::string& name, const std::string& description)
    : id(id), name(name), description(description) {}

int UserRole::getId() const { return id; }
std::string UserRole::getName() const { return name; }
std::string UserRole::getDescription() const { return description; }

void UserRole::setName(const std::string& name) { this->name = name; }
void UserRole::setDescription(const std::string& description) { this->description = description; }
