#pragma once
#include <SFML/Graphics.hpp>
#include <memory>

namespace rogalique
{
    class Player;
    class Menu;
    class CameraComponent;
    
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
        void Update(float deltaTime);
        void Draw();
        
        Player* m_player = nullptr;
        std::unique_ptr<Menu> m_menu;
        std::unique_ptr<CameraComponent> m_camera;
        sf::View m_gameView;
        bool m_inMenu = true;
        bool m_useCamera = false;
    };
    
    extern Application* g_Application;
    
    constexpr int SCREEN_WIDTH = 1024;
    constexpr int SCREEN_HEIGHT = 768;
    
    // Измени эти значения для теста камеры
    constexpr float WORLD_WIDTH = SCREEN_WIDTH * 2.0f;   // 1.0 = без камеры, 1.5 = с камерой
    constexpr float WORLD_HEIGHT = SCREEN_HEIGHT * 2.0f; // 1.0 = без камеры, 1.5 = с камерой
}
