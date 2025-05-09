#pragma once

#include <SFML/Network.hpp>
#include <string>
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
    std::string receiveRawResponse();
};
