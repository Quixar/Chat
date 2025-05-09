#include "server.h"

Server::Server(int port) : dataContext("mysqlx://root:INnoVation@localhost:33060", "Chat")
{
    this->port = port;
    auto ip = sf::IpAddress::resolve("127.0.0.1");

    if (listener.listen(port, *ip) != sf::Socket::Status::Done)
    {
        throw std::runtime_error("Failed to bind port");
    }
    selector.add(listener);
    std::cout << "Server started on " << ip->toString()<< ":" << port << "\n";
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
        std::cout << "New client connected: " << client->getRemoteAddress()->toString() << "\n";
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
        std::cout << "Received from client " << index << ": " << message << "\n";

        // Проверка на начало строки, которая указывает на запрос регистрации
        if (message.rfind("REG:", 0) == 0)
        {
            std::string registrationData = message.substr(4);
            std::cout << "Registration data received: " << registrationData << "\n";

            std::istringstream stream(registrationData);
            std::string username, password, email;

            std::getline(stream, username);
            std::getline(stream, password);
            std::getline(stream, email);

            // Проверяем, существует ли уже такой пользователь
            if (dataContext.usernameExists(username))
            {
                std::string errorMessage = "ERROR:USERNAME_EXISTS";
                if (socket.send(errorMessage.c_str(), errorMessage.size()) != sf::Socket::Status::Done)
                {
                    std::cerr << "Failed to send username exists error to client\n";
                }
                std::cout << "Username already exists: " << username << "\n";
            }
            else
            {
                if (!username.empty() && !password.empty() && !email.empty())
                {
                    dataContext.addUser(User(username, email, password, User::getCurrentDateTime(), 1));
                    std::cout << "New user registered: " << username << "\n";

                    std::string successMessage = "REG_SUCCESS";
                    if (socket.send(successMessage.c_str(), successMessage.size()) != sf::Socket::Status::Done)
                    {
                        std::cerr << "Failed to send success message to client\n";
                    }
                }
                else
                {
                    std::string errorMessage = "ERROR:INVALID_DATA";
                    if (socket.send(errorMessage.c_str(), errorMessage.size()) != sf::Socket::Status::Done)
                    {
                        std::cerr << "Failed to send invalid data error to client\n";
                    }
                    std::cout << "Invalid registration data format\n";
                }
            }
        }
        else if (message.rfind("LOGIN:", 0) == 0)
        {
            std::string loginData = message.substr(6);
            std::cout << "Login attempt: " << loginData << "\n";

            std::istringstream stream(loginData);
            std::string username, password;

            std::getline(stream, username);
            std::getline(stream, password);

            if (dataContext.checkPassword(username, password))
            {
                std::string successMessage = "LOGIN_SUCCESS";
                if (socket.send(successMessage.c_str(), successMessage.size()) != sf::Socket::Status::Done)
                {
                    std::cerr << "Failed to send success message to client\n";
                }
                std::cout << "User logged in successfuly\n";
            }
            else
            {
                std::string errorMessage = "ERROR:INVALID_PASSWORD";
                if (socket.send(errorMessage.c_str(), errorMessage.size()) != sf::Socket::Status::Done)
                {
                    std::cerr << "Failed to send user not found error to client\n";
                }
                std::cout << "User not found: " << username << "\n";
            }
        }
    }
    else if (status == sf::Socket::Status::Disconnected)
    {
        std::cout << "Client disconnected: " << socket.getRemoteAddress()->toString() << "\n";
        selector.remove(socket);
        clients.erase(clients.begin() + index);
    }
}
