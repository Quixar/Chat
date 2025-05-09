#pragma once

#include <mysqlx/xdevapi.h>
#include <memory>
#include <string>
#include <vector>
#include "User.h"
#include "UserRole.h"

class DataContext {
private:
    std::unique_ptr<mysqlx::Session> session; // Умный указатель на сессию
    mysqlx::Schema schema;  // Схема базы данных

public:
    // Конструктор, который инициализирует соединение с базой данных
    DataContext(const std::string& uri, const std::string& dbName);

    // Деструктор, закрывающий сессию
    ~DataContext();

    // Получение схемы базы данных
    mysqlx::Schema& getSchema();

    // Получение таблицы по имени
    mysqlx::Table getTable(const std::string& tableName);

    // Метод для добавления нового пользователя
    void addUser(const User& user);

    // Метод для получения всех пользователей (можно дополнить SQL запросом)
    std::vector<User> getUsers();

    // Метод для получения пользователя по ID
    User getUserById(int id);

    // Метод для добавляния роли пользователя
    void addUserRole(const UserRole& user_role);

    std::vector<UserRole> getUserRoles();

    UserRole getUserRoleById(int id);

    bool usernameExists(const std::string& username);

    bool checkPassword(const std::string& username, const std::string& password);
};
