#pragma once

#include <SFML/Network.hpp>
#include "../Entities/DataContext.h"
#include "../Entities/User.h"
#include "../Entities/UserRole.h"
#include <iostream>
#include <memory>
#include <vector>
#include <string>
#include <unordered_map>

class Server
{
private:
    DataContext dataContext;
    sf::TcpListener listener;
    sf::SocketSelector selector;
    std::vector<std::unique_ptr<sf::TcpSocket>> clients;
    std::unordered_map<sf::TcpSocket*, std::string> socketToUsername;  
    int port;

    void acceptNewClient();
    void handleClient(std::size_t index);
public:
    Server(int port);
    void run();
};

