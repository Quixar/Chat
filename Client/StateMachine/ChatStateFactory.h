#pragma once

#include "IStateFactory.h"
#include "ChatState.h"

class ChatStateFactory : public IStateFactory {
public:
    ChatStateFactory(sf::RenderWindow& window, StateMachine& sm, NetClient& client);

    std::unique_ptr<State> create() override;

private:
    sf::RenderWindow& window;
    StateMachine& stateMachine;
    NetClient& netClient;
};

