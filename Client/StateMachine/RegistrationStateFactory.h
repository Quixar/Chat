#pragma once

#include "IStateFactory.h"
#include "RegistrationState.h"

class RegistrationStateFactory : public IStateFactory {
public:
    RegistrationStateFactory(sf::RenderWindow& window, StateMachine& sm, NetClient& client);

    void init(IStateFactory& loginFactory, IStateFactory& chatFactory);

    std::unique_ptr<State> create() override;

private:
    sf::RenderWindow& window;
    StateMachine& stateMachine;
    NetClient& netClient;

    IStateFactory* loginFactory = nullptr;
    IStateFactory* chatFactory = nullptr;
};

