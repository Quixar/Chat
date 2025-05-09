#pragma once

#include "../NetClient//NetClient.h"
#include "StateMachine.h"
#include "LoginState.h"
#include "State.h"
#include <SFML/Window.hpp>

class RegistrationState : public State
{
private:

    StateMachine& stateMachine;

    NetClient& netClient;

    sf::RenderWindow& window;

    sf::Font font;
    sf::Text title;

    sf::Text login_lable;

    sf::Text sign_up_text;

    sf::Text username_lable;
    sf::Text username_text;
    sf::Text error_lable;

    sf::Text password_lable;
    sf::Text password_text;

    sf::Text email_lable;
    sf::Text email_text;

    sf::RectangleShape username_box;
    sf::RectangleShape password_box;
    sf::RectangleShape email_box;
    sf::RectangleShape sign_up_box;

    bool is_username_selected;
    bool is_password_selected;
    bool is_email_selected;
    bool is_sign_up_selected;
    bool is_registration_submitted;
    bool is_login_selected;

    std::string username;
    std::string password;
    std::string email;
public:
    RegistrationState(sf::RenderWindow& window, StateMachine& sm, NetClient& netClient);
    void handleEvent(const sf::Event& event) override;
    void render(sf::RenderWindow& window) override;
    void update() override;
};
