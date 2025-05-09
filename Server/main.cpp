#include "NetServer/server.h"
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

int main()
{
    Server server(53000);
    server.run();
    return 0;
}
