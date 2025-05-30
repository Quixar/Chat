#pragma once

#include <SFML/Network.hpp>
#include <string>
#include <sstream>
#include <iostream>

class NetClient
{
private:
    sf::TcpSocket socket;
    std::string server_ip;
    int server_port;
public:
    NetClient(const std::string& ip, int port);
    bool connect();
    bool sendRegistrationData(const std::string& username, const std::string& password, const std::string& email);
    bool sendLoginData(const std::string& username, const std::string& password);
    void disconnect();
    bool isRegSuccessful();
    bool isLogSuccessful();
    bool sendMessage(const std::string& receiver, const std::string& message);
    bool sendRawMessage(const std::string& message);
    std::string receiveRawResponse();
    std::string getActiveUsers();
    std::vector<std::string> fetchMessagesFrom(const std::string& sender); 
    bool hasNewMessagesFrom(const std::string& sender);
};
