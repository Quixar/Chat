#pragma once

#include <mysqlx/xdevapi.h>
#include <memory>
#include <string>
#include <vector>
#include "User.h"
#include "UserRole.h"

class DataContext {
private:
    std::unique_ptr<mysqlx::Session> session; 
    mysqlx::Schema schema;  

public:
    DataContext(const std::string& uri, const std::string& dbName);

    ~DataContext();

    mysqlx::Schema& getSchema();

    mysqlx::Table getTable(const std::string& tableName);

    void addUser(const User& user);

    void updateUserActive(const std::string & username, bool is_active);

    std::vector<User> getUsers();

    User getUserById(int id);

    void addUserRole(const UserRole& user_role);

    std::vector<UserRole> getUserRoles();

    UserRole getUserRoleById(int id);

    bool usernameExists(const std::string& username);

    bool checkPassword(const std::string& username, const std::string& password);

    void storeMessage(const std::string& sender, const std::string& receiver, const std::string& message);

    std::vector<std::string> fetchMessages(const std::string& sender, const std::string& receiver);

    void markMessagesAsDelivered(const std::string& sender, const std::string& receiver);

    void updateLastLogin(const std::string& username);

    template<typename T>
    void logValue(const T& value)
    {
        std::cout << "[LOG]: " << value << "\n";
    }
};
