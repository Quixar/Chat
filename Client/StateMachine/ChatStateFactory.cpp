#include "ChatStateFactory.h"

ChatStateFactory::ChatStateFactory(sf::RenderWindow& window, StateMachine& sm, NetClient& client)
    : window(window), stateMachine(sm), netClient(client)
{}

std::unique_ptr<State> ChatStateFactory::create() {
    return std::make_unique<ChatState>(window, stateMachine, netClient);
}

