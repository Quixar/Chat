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
