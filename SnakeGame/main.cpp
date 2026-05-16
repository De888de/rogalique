#include "Constants.h"
#include "Game.h"
#include <SFML/Graphics.hpp>

using namespace SnakeGame;

int main()
{
    sf::RenderWindow window(
        sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT),
        "Snake Game"
    );

    SnakeGame::Game game;

    sf::Clock clock;
    float lastTime = 0.0f;

    while (window.isOpen())
    {
        float currentTime = clock.getElapsedTime().asSeconds();
        float deltaTime = currentTime - lastTime;
        lastTime = currentTime;

        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        
        game.HandleInput();
        game.Update(deltaTime);

        window.clear();
        game.Draw(window);
        window.display();
    }

    return 0;
}