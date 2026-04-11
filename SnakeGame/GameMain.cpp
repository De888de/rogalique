#include <SFML/Graphics.hpp>
#include "Game.h"

// Глобальный указатель
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

        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
                window.close();
        }

        game.UpdateGame(timeDelta, window);
    }

    return 0;
}