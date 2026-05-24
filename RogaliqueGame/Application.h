#pragma once
#include <SFML/Graphics.hpp>

namespace RoqaliqueGame
{
    class Application
    {
    public:
        Application();
        ~Application();
        void Run();
        void StartGame();
        void ReturnToMenu();

        sf::RenderWindow window;
    };
}
