#include "User.h"

User::User(const std::string& username, const std::string& email,
           const std::string& password_hash, const std::string& created_at,
           int role_id, bool is_active, const std::string& last_login)
    : id(next_id++), username(username), email(email), password_hash(password_hash),
      created_at(created_at), role_id(role_id), is_active(is_active), last_login(last_login) {}

User::User(int id, const std::string& username, const std::string& email,
           const std::string& password_hash, const std::string& created_at,
           int role_id, bool is_active, const std::string& last_login)
    : id(id), username(username), email(email), password_hash(password_hash),
      created_at(created_at), role_id(role_id), is_active(is_active), last_login(last_login) {}

int User::getId() const { return id; }
std::string User::getUsername() const { return username; }
std::string User::getEmail() const { return email; }
std::string User::getPasswordHash() const { return password_hash; }
std::string User::getCreatedAt() const { return created_at; }
int User::getRoleId() const { return role_id; }
bool User::getIsActive() const { return is_active; }
std::string User::getLastLogin() const { return last_login; }

void User::setUsername(const std::string& username) { this->username = username; }
void User::setEmail(const std::string& email) { this->email = email; }
void User::setPasswordHash(const std::string& password_hash) { this->password_hash = password_hash; }
void User::setRoleId(int role_id) { this->role_id = role_id; }
void User::setIsActive(bool is_active) { this->is_active = is_active; }
void User::setLastLogin(const std::string& lastLogin) { this->last_login = lastLogin; }

std::string User::getCurrentDateTime() {
    auto now = std::chrono::system_clock::now();
    std::time_t t = std::chrono::system_clock::to_time_t(now);
    std::tm tm = *std::gmtime(&t);  

    std::ostringstream oss;
    oss << std::put_time(&tm, "%Y-%m-%d %H:%M:%S");
    return oss.str();
}

int User::next_id = 1;
