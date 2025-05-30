#include "DataContext.h"

DataContext::DataContext(const std::string& uri, const std::string& dbName)
    : session(std::make_unique<mysqlx::Session>(uri)),  
      schema(session->getSchema(dbName))  
{
    try {
        std::cout << "Connected to database: " << dbName << std::endl;
    } catch (const mysqlx::Error& err) {
        std::cerr << "Error connecting to the database: " << err.what() << std::endl;
        throw;
    }
}

DataContext::~DataContext() {
    try {
        if (session) {
            session->close();
            std::cout << "Connection closed." << std::endl;
        }
    } catch (const mysqlx::Error& err) {
        std::cerr << "Error while closing the session: " << err.what() << std::endl;
    }
}

mysqlx::Schema& DataContext::getSchema() {
    return schema;
}

mysqlx::Table DataContext::getTable(const std::string& tableName) {
    return schema.getTable(tableName);
}

void DataContext::addUser(const User& user) {
    try {
        mysqlx::Table usersTable = getTable("users");
        
        usersTable.insert("username", "email", "password_hash", "created_at", "role_id")
            .values(user.getUsername(), user.getEmail(), user.getPasswordHash(),
                    User::getCurrentDateTime(), user.getRoleId())
            .execute();
        
        std::cout << "User added: " << user.getUsername() << std::endl;
    } catch (const mysqlx::Error& err) {
        std::cerr << "Error adding user: " << err.what() << std::endl;
    }
}

std::vector<User> DataContext::getUsers() {
    std::vector<User> users;
    try {
        mysqlx::Table usersTable = getTable("users");
        mysqlx::RowResult result = usersTable.select("id", "username", "email", "password_hash", "created_at", "role_id", "is_active", "last_login")
            .execute();

        for (auto row : result) {
            int id = row[0].get<int>();
            std::string username = row[1].get<std::string>();
            std::string email = row[2].get<std::string>();
            std::string passwordHash = row[3].get<std::string>();
            std::string createdAt = row[4].get<std::string>();
            int roleId = row[5].get<int>();
            bool is_active = row[6].get<bool>();
            std::string last_login = row[7].isNull() ? "" : row[7].get<std::string>();


            UserRole role = getUserRoleById(roleId);

            users.push_back(User(id, username, email, passwordHash, createdAt, roleId, is_active, last_login));
        }
    } catch (const mysqlx::Error& err) {
        std::cerr << "Error fetching users: " << err.what() << std::endl;
    }
    return users;
}

User DataContext::getUserById(int id) {
    User user;
    try {
        mysqlx::Table usersTable = getTable("users");
        mysqlx::RowResult result = usersTable.select("id", "username", "email", "password_hash", "created_at", "role_id", "is_active")
            .where("id = :id").bind("id", id)
            .execute();

        for (auto row : result) {
            int userId = row[0].get<int>();
            std::string username = row[1].get<std::string>();
            std::string email = row[2].get<std::string>();
            std::string passwordHash = row[3].get<std::string>();
            std::string createdAt = row[4].get<std::string>();
            int roleId = row[5].get<int>();
            bool is_active = row[6].get<bool>();

            UserRole role = getUserRoleById(roleId);

            user = User(userId, username, email, passwordHash, createdAt, roleId, is_active);
        }
    } catch (const mysqlx::Error& err) {
        std::cerr << "Error fetching user by ID: " << err.what() << std::endl;
    }
    return user;
}

void DataContext::addUserRole(const UserRole& role) {
    try {
        mysqlx::Table rolesTable = getTable("user_roles");
        rolesTable.insert("name", "description")
            .values(role.getName(), role.getDescription())
            .execute();
    } catch (const mysqlx::Error& err) {
        std::cerr << "Error adding user role: " << err.what() << std::endl;
    }
}

std::vector<UserRole> DataContext::getUserRoles()
{
    std::vector<UserRole> roles;
    try
    {
        mysqlx::Table roles_table = getTable("user_roles");
        mysqlx::RowResult result = roles_table.select("id", "name", "description").execute();

        for (auto row : result)
        {
            int id = row[0].get<int>();
            std::string name = row[1].get<std::string>();
            std::string description = row[2].get<std::string>();

            roles.push_back(UserRole(id, name, description));
        }
    }
    catch (const mysqlx::Error& err)
    {
        std::cerr << "Error fetching all user roles: " << err.what() << std::endl;
    }
    return roles;
}

UserRole DataContext::getUserRoleById(int id)
{
    UserRole role(0, "", "");

    try
    {
        mysqlx::Table role_table = getTable("user_roles");
        mysqlx::RowResult result = role_table.select("id", "name", "description")
            .where("id = :id").bind("id", id).
            execute();

        if (result.count() > 0) 
        {
                auto row = result.fetchOne();
                int id = row[0].get<int>();
                std::string name = row[1].get<std::string>();
                std::string description = row[2].get<std::string>();
                role = UserRole(id, name, description);
        }
    }
    catch (mysqlx::Error& err)
    {
        std::cerr << "Error fetching user role by ID: " << err.what() << std::endl;
    }
    return role;
}

bool DataContext::usernameExists(const std::string& username)
{
    try
    {
        auto users = session->getSchema("Chat").getTable("users");
        auto result = users.select("username")
             .where("username = :username")
             .bind("username", username)
             .execute();
        return result.count() > 0;
    }
    catch(const mysqlx::Error& err)
    {
        std::cerr << "MySQL error in usernameExists: " << err.what() << "\n";
        return false;
    }
}

bool DataContext::checkPassword(const std::string& username, const std::string& password)
{
    try
    {
        auto users = session->getSchema("Chat").getTable("users");
        auto result = users.select("password_hash")
            .where("username = :username")
            .bind("username", username)
            .execute();
        
        if (mysqlx::Row row = result.fetchOne())
        {
            std::string storedPassword = row[0].get<std::string>();
            return storedPassword == password;
        }
    }
    catch (const mysqlx::Error& err)
    {
        std::cerr << "MySQL error in checkPassword: " << err.what() << "\n";
        return false;
    }
    return false;
}

void DataContext::updateUserActive(const std::string& username, bool isActive)
{
    try
    {
        mysqlx::Schema db = session->getSchema("chat"); 
        mysqlx::Table users = db.getTable("users");

        users.update()
            .set("is_active", isActive ? 1 : 0)
            .where("username = :username")
            .bind("username", username)
            .execute();}
    catch (mysqlx::Error& e)
    {
        std::cerr << "MySQL error in setUserActive: " << e.what() << "\n";
    }
}

void DataContext::updateLastLogin(const std::string& username)
{
    try
    {
        mysqlx::Schema db = session->getSchema("chat");
        mysqlx::Table users = db.getTable("users");

        users.update()
            .set("last_login", User::getCurrentDateTime())
            .where("username = :username")
            .bind("username", username)
            .execute();
        
    }
    catch (mysqlx::Error& e)
    {
        std::cerr << "MySQL error in updateLastLogin: " << e.what() << "\n";
    }
}

void DataContext::storeMessage(const std::string& sender, const std::string& receiver, const std::string& content)
{
    try
    {
        mysqlx::Schema db = session->getSchema("chat");
        mysqlx::Table messages = db.getTable("messages");

        messages.insert("sender", "receiver", "content")
                .values(sender, receiver, content)
                .execute();

        std::cout << "Message from '" << sender << "' to '" << receiver << "' stored successfully.\n";
    }
    catch (const mysqlx::Error& err)
    {
        std::cerr << "MySQL Error (storeMessage): " << err.what() << std::endl;
    }
}

std::vector<std::string> DataContext::fetchMessages(const std::string& sender, const std::string& receiver) {
    std::vector<std::string> messages;

    try {
        mysqlx::Table messagesTable = getTable("messages");
        mysqlx::RowResult result = messagesTable.select("content")
            .where("sender = :sender AND receiver = :receiver AND delivered = 0")
            .bind("sender", sender)
            .bind("receiver", receiver)
            .execute();

        for (auto row : result) {
            messages.push_back(row[0].get<std::string>());
        }
    } catch (const mysqlx::Error& err) {
        std::cerr << "Error fetching messages: " << err.what() << std::endl;
    }

    return messages;
}

void DataContext::markMessagesAsDelivered(const std::string& sender, const std::string& receiver)
{
    try
    {
        mysqlx::Schema db = session->getSchema("chat");
        mysqlx::Table messages = db.getTable("messages");

        messages.update()
                .set("delivered", 1)
                .where("sender = :sender AND receiver = :receiver AND delivered = 0")
                .bind("sender", sender)
                .bind("receiver", receiver)
                .execute();

        std::cout << "Marked messages from '" << sender << "' to '" << receiver << "' as delivered.\n";
    }
    catch (const mysqlx::Error& err)
    {
       
        std::cerr << "MySQL Error (markMessagesAsDelivered): " << err.what() << std::endl;
    }
}
