#include <SFML/Graphics.hpp>
#include "Game.h"


ArkanoidGame::Game* g_Game = nullptr;

namespace ArkanoidGame
{
    Game& GetGame()
    {
        if (g_Game == nullptr)
        {
            static Game dummy;
            return dummy;
        }
        return *g_Game;
    }
}

int main()
{
    sf::RenderWindow window(sf::VideoMode(800, 600), "Arkanoid");
    window.setFramerateLimit(60);

    ArkanoidGame::Game game;
    g_Game = &game;

    sf::Clock clock;

    while (window.isOpen())
    {
        float timeDelta = clock.restart().asSeconds();


        game.UpdateGame(timeDelta, window);
    }

    return 0;
}