#include "RegistrationStateFactory.h"

RegistrationStateFactory::RegistrationStateFactory(sf::RenderWindow& window, StateMachine& sm, NetClient& client)
    : window(window), stateMachine(sm), netClient(client)
{}

void RegistrationStateFactory::init(IStateFactory& loginFactoryRef, IStateFactory& chatFactoryRef) {
    loginFactory = &loginFactoryRef;
    chatFactory = &chatFactoryRef;
}

std::unique_ptr<State> RegistrationStateFactory::create() {
    if (!loginFactory) {
        throw std::runtime_error("RegistrationStateFactory not initialized with LoginStateFactory");
    }
    return std::make_unique<RegistrationState>(window, stateMachine, netClient, *loginFactory, *chatFactory);
}

