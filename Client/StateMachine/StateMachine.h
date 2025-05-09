#pragma once

#include "State.h"
#include <memory>
#include <stack>

class StateMachine
{
private:
    std::stack<std::unique_ptr<State>> states;
public:
    void pushState(std::unique_ptr<State> state);
    void popState();
    void changeState(std::unique_ptr<State> state);
    void handleEvent(sf::Event event);
    void update();
    void render(sf::RenderWindow& window);
};
