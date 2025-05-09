#pragma once

#include <string>
#include <iostream>

class UserRole {
private:
    int id;
    std::string name;
    std::string description;

public:
    UserRole(int id, const std::string& name, const std::string& description);

    int getId() const;
    std::string getName() const;
    std::string getDescription() const;

    void setName(const std::string& name);
    void setDescription(const std::string& description);
};
