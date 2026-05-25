#pragma once
#include <SFML/Graphics.hpp>
#include <memory>

namespace rogalique
{
    class Player;

    class Application
    {
    public:
        Application();
        ~Application();

        void Run();
        void StartGame();
        void ReturnToMenu();

        sf::RenderWindow window;

    private:
        void Update(float deltaTime);
        void Draw();
        void ShowLogoSplash();

        std::unique_ptr<Player> m_player;
    };

    extern Application* g_Application;

    constexpr int SCREEN_WIDTH = 1024;
    constexpr int SCREEN_HEIGHT = 768;
}