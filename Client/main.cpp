#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include "StateMachine/StateMachine.h"
#include "StateMachine/RegistrationStateFactory.h"
#include "StateMachine/ChatStateFactory.h"
#include "StateMachine/LoginStateFactory.h"
#include "NetClient/NetClient.h"

int main()
{
    sf::RenderWindow window(sf::VideoMode({ 800, 600 }), "Login Example");
    window.setFramerateLimit(60);
    StateMachine stateMachine;
    NetClient netClient("127.0.0.1", 53000);

    netClient.connect();

    LoginStateFactory loginFactory(window, stateMachine, netClient);
    RegistrationStateFactory registrationFactory(window, stateMachine, netClient);
    ChatStateFactory chatFactory(window, stateMachine, netClient);

    loginFactory.init(registrationFactory, chatFactory);
    registrationFactory.init(loginFactory, chatFactory);  

    stateMachine.pushState(loginFactory.create());

    while (window.isOpen())
    {
        while (auto event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
            {
                window.close();
            }
            else
            {
                stateMachine.handleEvent(*event);
            }
        }

        stateMachine.update();

        stateMachine.render(window);
        window.display();
    }
}
