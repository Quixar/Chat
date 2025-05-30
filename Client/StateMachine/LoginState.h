#pragma once

#include "../NetClient/NetClient.h"
#include "State.h"
#include "Wrapper.h"
#include "StateMachine.h"
#include "RegistrationState.h"
#include "ChatState.h"
#include "LoginStateFactory.h"
#include <SFML/Graphics.hpp>
#include <string>

class LoginState : public State {
private:
    StateMachine& stateMachine;

    NetClient& netClient;
    
    sf::RenderWindow& window;

    sf::Font font;
    sf::Text title;

    sf::Text error_lable;

    sf::Text sign_in_text;

    sf::Text registration_lable;
    sf::Text username_text;
    sf::Text username_lable;

    sf::Text password_text;
    sf::Text password_lable;

    sf::RectangleShape username_box;
    sf::RectangleShape password_box;
    sf::RectangleShape sign_in_box;

    bool is_username_selected;
    bool is_password_selected;
    bool is_sign_in_selected;
    bool is_sign_in_submitted;
    bool is_register_selected;

    Wrapper<std::string> username;
    Wrapper<std::string> password;

    IStateFactory& registrationFactory;
    IStateFactory& chatFactory;

public:
    LoginState(sf::RenderWindow& window, StateMachine& sm, NetClient& netClient, IStateFactory& regFactory, IStateFactory& chatFactory);

    void handleEvent(const sf::Event& event) override;
    void update() override;
    void render(sf::RenderWindow& window) override;
};
