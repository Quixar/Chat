#include "server.h"

Server::Server(int port) : dataContext("mysqlx:
{
    this->port = port;
    auto ip = sf::IpAddress::resolve("127.0.0.1");

    if (listener.listen(port, *ip) != sf::Socket::Status::Done)
    {
        throw std::runtime_error("Failed to bind port");
    }
    selector.add(listener);
    dataContext.logValue("Server started on " +  ip->toString() + ":" + std::to_string(port));
}

void Server::run()
{
    while(true)
    {
        if (selector.wait())
        {
            if (selector.isReady(listener))
            {
                acceptNewClient();
            }
            else
            {
                for (int i = clients.size() - 1; i >= 0; --i) 
                {
                    if (selector.isReady(*clients[i])) 
                    {
                        handleClient(i);
                    }
                }
            }
        }
    }
}

void Server::acceptNewClient()
{
    auto client = std::make_unique<sf::TcpSocket>();
    if (listener.accept(*client) == sf::Socket::Status::Done)
    {
        selector.add(*client);
        dataContext.logValue("New client connected: " + client->getRemoteAddress()->toString() + "\n");
        clients.push_back(std::move(client));
    }
}

void Server::handleClient(std::size_t index)
{
    char buffer[1024];
    std::size_t received;
    sf::TcpSocket& socket = *clients[index];

    sf::Socket::Status status = socket.receive(buffer, sizeof(buffer), received);

    if (status == sf::Socket::Status::Done)
    {
        std::string message(buffer, received);
        dataContext.logValue("Received from client " + std::to_string(index) + ":" + message + "\n");

        if (message.rfind("REG:", 0) == 0)
        {
            std::string registrationData = message.substr(4);
            std::istringstream stream(registrationData);
            std::string username, password, email;

            std::getline(stream, username);
            std::getline(stream, password);
            std::getline(stream, email);

            if (dataContext.usernameExists(username))
            {
                std::string errorMessage = "ERROR:USERNAME_EXISTS";
                socket.send(errorMessage.c_str(), errorMessage.size());
            }
            else if (!username.empty() && !password.empty() && !email.empty())
            {
                dataContext.addUser(User(username, email, password, User::getCurrentDateTime(), 1));
                dataContext.updateUserActive(username, true);
                socketToUsername[&socket] = username;

                std::string successMessage = "REG_SUCCESS";
                socket.send(successMessage.c_str(), successMessage.size());
            }
            else
            {
                std::string errorMessage = "ERROR:INVALID_DATA";
                socket.send(errorMessage.c_str(), errorMessage.size());
            }
        }
        else if (message.rfind("LOGIN:", 0) == 0)
        {
            std::string loginData = message.substr(6);
            std::istringstream stream(loginData);
            std::string username, password;

            std::getline(stream, username);
            std::getline(stream, password);

            if (dataContext.checkPassword(username, password))
            {
                dataContext.updateUserActive(username, true);
                socketToUsername[&socket] = username;

                std::string successMessage = "LOGIN_SUCCESS";
                socket.send(successMessage.c_str(), successMessage.size());
            }
            else
            {
                std::string errorMessage = "ERROR:INVALID_PASSWORD";
                socket.send(errorMessage.c_str(), errorMessage.size());
            }
        }
        else if (message == "GET_ACTIVE_USERS")
        {
            std::string response;
            for (auto& user : dataContext.getUsers())
            {
                if (user.getIsActive())
                {
                    response += user.getUsername() + '|';
                }
            }
            if (response.empty()) response = "NONE";

            socket.send(response.c_str(), response.size());
        }
        else if (message.rfind("MSG:", 0) == 0)
        {
            std::string msgData = message.substr(4);
            std::istringstream stream(msgData);
            std::string receiver, msgText;

            std::getline(stream, receiver);
            std::getline(stream, msgText);

            auto senderIt = socketToUsername.find(&socket);
            if (senderIt == socketToUsername.end())
            {
                std::string error = "ERROR:NOT_LOGGED_IN";
                socket.send(error.c_str(), error.size());
                return;
            }

            std::string sender = senderIt->second;
            dataContext.storeMessage(sender, receiver, msgText);

            std::string confirmation = "MSG_STORED";
            socket.send(confirmation.c_str(), confirmation.size());

            for (auto& clientSocket : clients)
            {
                auto it = socketToUsername.find(clientSocket.get());
                if (it != socketToUsername.end() && it->second == receiver)
                {
                    std::string outgoingMessage = "MSG_FROM:" + sender + "\n" + msgText;
                    clientSocket->send(outgoingMessage.c_str(), outgoingMessage.size());
                    break;
                }
            }
        }
        else if (message.rfind("GET_MSGS_FROM:", 0) == 0) 
        {
            std::string sender = message.substr(14);
            auto receiverIt = socketToUsername.find(&socket);
            if (receiverIt != socketToUsername.end()) 
            {
                std::string receiver = receiverIt->second;

                std::vector<std::string> messages = dataContext.fetchMessages(sender, receiver);

                if (!messages.empty()) 
                {
                    std::ostringstream oss;
                    for (const auto& msg : messages) 
                    {
                        oss << msg << "|";
                    }

                    std::string reply = oss.str();
                    if (!reply.empty()) 
                    {
                        if (socket.send(reply.c_str(), reply.size()) != sf::Socket::Status::Done) 
                        {
                            dataContext.logValue("Failed to send messages from " + sender + "to " + receiver);
                        }
                        dataContext.markMessagesAsDelivered(sender, receiver);
                    }
                    else
                    {
                        std::string emptyMsg = "NO_MSGS";
                        socket.send(emptyMsg.c_str(), emptyMsg.size());
                    }
                } 
                else 
                {
                    dataContext.logValue("No messages to send from " + sender + "to " + receiver + "\n");
                }
            }
        }
        else if (message.rfind("CHECK_MSGS_FROM:", 0) == 0)
        {
            std::string sender = message.substr(16); 
            auto receiverIt = socketToUsername.find(&socket);
            if (receiverIt != socketToUsername.end())
            {
                std::string receiver = receiverIt->second;
                auto pendingMessages = dataContext.fetchMessages(sender, receiver);

                std::string response = pendingMessages.empty() ? "NO_MSGS" : "HAS_MSGS";
                socket.send(response.c_str(), response.size());
            }
        }
    }
    else if (status == sf::Socket::Status::Disconnected)
    {
        dataContext.logValue("Client disconnected: " + socket.getRemoteAddress()->toString() + "\n");

        auto it = socketToUsername.find(&socket);
        if (it != socketToUsername.end())
        {
            dataContext.updateUserActive(it->second, false);
            dataContext.updateLastLogin(it->second);
            socketToUsername.erase(it);
        }

        selector.remove(socket);
        clients.erase(clients.begin() + index);
    }
}
