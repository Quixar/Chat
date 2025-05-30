#include "LoginStateFactory.h"

LoginStateFactory::LoginStateFactory(sf::RenderWindow& window, StateMachine& sm, NetClient& netClient)
    : window(window), stateMachine(sm), netClient(netClient)
{}

void LoginStateFactory::init(IStateFactory& regFactory, IStateFactory& chatFactory) {
    registrationFactory = &regFactory;
    this->chatFactory = &chatFactory;
}

std::unique_ptr<State> LoginStateFactory::create() {
    if (!registrationFactory || !chatFactory) {
        throw std::runtime_error("LoginStateFactory not initialized with dependencies");
    }
    return std::make_unique<LoginState>(window, stateMachine, netClient, *registrationFactory, *chatFactory);
}
