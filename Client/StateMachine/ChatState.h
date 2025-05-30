#pragma once

#include <SFML/Graphics.hpp>
#include "State.h"
#include "StateMachine.h"
#include "../NetClient/NetClient.h"

class ChatState : public State {
public:
    ChatState(sf::RenderWindow& window, StateMachine& sm, NetClient& client);

    void handleEvent(const sf::Event& event) override;
    void update() override;
    void render(sf::RenderWindow& window) override;

private:
    sf::RenderWindow& window;
    StateMachine& stateMachine;
    NetClient& client;
    sf::Clock updateClock;
    float updateInterval = 1.f;

    std::vector<std::string> activeUsers;
    int selectedUserIndex = -1;

    sf::Font font;

    struct UserEntry {
        sf::RectangleShape background;
        sf::Text username;
        UserEntry(const std::string& name, const sf::Font& font, float posY) : username(font){
            background.setSize({250.f, 50.f});
            background.setFillColor(sf::Color(230, 230, 230));
            background.setPosition({0.f, posY});

            username.setFont(font);
            username.setCharacterSize(16);
            username.setFillColor(sf::Color::Black);
            username.setString(name);
            username.setPosition({10.f, posY + 15.f});
        }
    };
    std::vector<UserEntry> userEntries;

    std::string inputText;
    sf::Text inputDisplay;
    sf::RectangleShape inputBox;

    sf::RectangleShape sendButton;
    sf::Text sendButtonText;

    std::vector<std::string> chatMessages;

    void fetchActiveUsers();
    void renderUserList();
    void renderChatWindow();
    void handleMouseClick(const sf::Vector2f& mousePos);

    void sendMessage();
};
