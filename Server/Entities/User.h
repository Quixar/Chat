#pragma once

#include <iostream>
#include <string>
#include <ctime>
#include <chrono>
#include <iomanip>
#include <sstream>

class User
{
private:
    int id;
    std::string username;
    std::string email;
    std::string password_hash;
    std::string created_at;
    int role_id;
    bool is_active;
    std::string last_login;
    static int next_id;

public:
    User(const std::string& username, const std::string& email,
         const std::string& passwordHash, const std::string& createdAt,
         int roleId, bool isActive = true, const std::string& lastLogin = "");

    User(int id,const std::string& username, const std::string& email,
         const std::string& passwordHash, const std::string& createdAt,
         int roleId, bool isActive = true, const std::string& lastLogin = "");

    User() 
        : id(0), username(""), email(""), password_hash(""), 
          created_at(getCurrentDateTime()), role_id(0), 
          is_active(true), last_login("") {}

    int getId() const;
    std::string getUsername() const;
    std::string getEmail() const;
    std::string getPasswordHash() const;
    std::string getCreatedAt() const;
    int getRoleId() const;
    bool getIsActive() const;
    std::string getLastLogin() const;

    void setUsername(const std::string& username);
    void setEmail(const std::string& email);
    void setPasswordHash(const std::string& password_hash);
    void setRoleId(int role_id);
    void setIsActive(bool is_active);
    void setLastLogin(const std::string& lastLogin); 

    static std::string getCurrentDateTime();
};
