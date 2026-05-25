#pragma once
#include <SFML/Graphics.hpp>
#include <memory>

namespace rogalique
{
    class Player;
    class Menu;
    
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
        void ShowLogoSplash();
        
        Player* m_player = nullptr;
        std::unique_ptr<Menu> m_menu;
        bool m_inMenu = true;
    };
    
    extern Application* g_Application;
    
    constexpr int SCREEN_WIDTH = 1024;
    constexpr int SCREEN_HEIGHT = 768;
}
