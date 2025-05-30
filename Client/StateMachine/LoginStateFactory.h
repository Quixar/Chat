#pragma once

#include "IStateFactory.h"
#include "LoginState.h"
#include <SFML/Graphics.hpp>
#include "../NetClient/NetClient.h"
#include "StateMachine.h"

class LoginStateFactory : public IStateFactory {
public:
    LoginStateFactory(sf::RenderWindow& window, StateMachine& stateMachine, NetClient& netClient);

    void init(IStateFactory& regFactory, IStateFactory& chatFactory);

    std::unique_ptr<State> create() override;

private:
    sf::RenderWindow& window;
    StateMachine& stateMachine;
    NetClient& netClient;

    IStateFactory* registrationFactory = nullptr;
    IStateFactory* chatFactory = nullptr;
};
