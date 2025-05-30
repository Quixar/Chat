#include "NetClient.h"

NetClient::NetClient(const std::string& ip, int port) : server_ip(ip), server_port(port) {}

bool NetClient::connect()
{
    auto ip = sf::IpAddress::resolve("127.0.0.1"); 

    sf::Socket::Status status = socket.connect(*ip, server_port);
    if (status != sf::Socket::Status::Done)
    {
        return false;
    }
    std::cout << "Connected succefully\n";
    return true;
}

void NetClient::disconnect()
{
    socket.disconnect();
}

bool NetClient::sendRegistrationData(const std::string& username, const std::string& password, const std::string& email)
{
    std::string data = "REG:" + username + "\n" + password + "\n" + email + "\n";
    if (socket.send(data.c_str(), data.size()) != sf::Socket::Status::Done)
    {
        std::cerr << "Failed to send registration data.\n";
        return false;
    }
    return true;
}

bool NetClient::sendLoginData(const std::string& username, const std::string& password)
{
    std::string data = "LOGIN:" + username + "\n" + password + "\n";
    if (socket.send(data.c_str(), data.size()) != sf::Socket::Status::Done)
    {
        std::cerr << "Failed to send login data";
        return false;
    }
    return true;
}

std::string NetClient::receiveRawResponse()
{
    char buffer[128];
    std::size_t received;

    if (socket.receive(buffer, sizeof(buffer), received) != sf::Socket::Status::Done)
    {
        return "ERROR:RECEIVE_FAILED";
    }
    return std::string(buffer, received);
}

bool NetClient::isRegSuccessful()
{
    return receiveRawResponse() == "REG_SUCCESS";
}

bool NetClient::isLogSuccessful()
{
    return receiveRawResponse() == "LOGIN_SUCCESS";
}
std::string NetClient::getActiveUsers()
{
    std::string data = "GET_ACTIVE_USERS";
    if (socket.send(data.c_str(), data.size()) != sf::Socket::Status::Done) {
        return "";
    }

    char buffer[1024];
    std::size_t received = 0;
    if (socket.receive(buffer, sizeof(buffer), received) != sf::Socket::Status::Done) {
        return "";
    }

    std::string response(buffer, received);
    return response;  
}

bool NetClient::sendMessage(const std::string& receiver, const std::string& message) {
    std::string msg = "MSG:" + receiver + "\n" + message;
    return sendRawMessage(msg);
}

bool NetClient::sendRawMessage(const std::string& message) {
    socket.send(message.c_str(), message.size());
    return true;
}

std::vector<std::string> NetClient::fetchMessagesFrom(const std::string& sender) 
{
    std::string request = "GET_MSGS_FROM:" + sender;
    if (socket.send(request.c_str(), request.size()) != sf::Socket::Status::Done) {
        std::cerr << "Failed to send GET_MSGS_FROM request\n";
        return {};
    }

    char buffer[4096];
    std::size_t received;

    sf::Socket::Status status = socket.receive(buffer, sizeof(buffer), received);
    if (status != sf::Socket::Status::Done) {
        std::cerr << "Failed to receive messages\n";
        return {};
    }

    std::string response(buffer, received);
    if (response == "NO_MSGS" || response.empty()) {
        return {}; 
    }

    std::vector<std::string> messages;
    std::stringstream ss(response);
    std::string line;

    while (std::getline(ss, line, '|')) {
        if (!line.empty())
            messages.push_back(sender + ": " + line);
    }

    return messages;
}
bool NetClient::hasNewMessagesFrom(const std::string& sender) {
    std::string request = "CHECK_MSGS_FROM:" + sender;
    if (socket.send(request.c_str(), request.size()) != sf::Socket::Status::Done) {
        std::cerr << "Failed to send CHECK_MSGS_FROM request\n";
        return false;
    }

    char buffer[128];
    std::size_t received;
    sf::Socket::Status status = socket.receive(buffer, sizeof(buffer), received);
    if (status != sf::Socket::Status::Done) {
        std::cerr << "Failed to receive CHECK_MSGS_FROM response\n";
        return false;
    }

    std::string response(buffer, received);
    return response == "HAS_MSGS";
}
