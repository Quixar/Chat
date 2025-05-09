#include "StateMachine.h"

void StateMachine::pushState(std::unique_ptr<State> state)
{
    states.push(std::move(state));
}

void StateMachine::popState()
{
    if (!states.empty())
    {
        states.pop();
    }
}

void StateMachine::changeState(std::unique_ptr<State> state)
{
    popState();
    pushState(std::move(state));

}

void StateMachine::handleEvent(sf::Event event)
{
    if (!states.empty())
    {
        states.top()->handleEvent(event);
    }
}

void StateMachine::update()
{
    if (!states.empty())
    {
        states.top()->update();
    }
}

void StateMachine::render(sf::RenderWindow& window)
{
    if (!states.empty())
    {
        states.top()->render(window);
    }
}
