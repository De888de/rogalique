#pragma once
#include <SFML/Graphics.hpp>
#include <memory>

namespace rogalique
{
    class Player;
    class Menu;
    class CameraComponent;
    class SoundSettingsWindow;
    
    class Application
    {
    public:
        Application();
        ~Application();
        
        void Run();
        void StartGame();
        void ReturnToMenu();
        void AddGold(int amount);
        void AddChest();
        
        sf::RenderWindow window;
        
    private:
        void ShowLogoSplash();
        void Update(float deltaTime);
        void Draw();
        void UpdateUI();
        
        Player* m_player = nullptr;
        std::unique_ptr<Menu> m_menu;
        std::unique_ptr<CameraComponent> m_camera;
        std::unique_ptr<SoundSettingsWindow> m_soundSettings;
        sf::View m_gameView;
        bool m_inMenu = true;
        bool m_inSoundSettings = false;
        bool m_useCamera = false;
        
        int m_gold = 0;
        int m_chestsCollected = 0;
        sf::Font m_uiFont;
        sf::Text m_goldText;
        sf::Text m_chestText;
    };
    
    extern Application* g_Application;
    
    constexpr int SCREEN_WIDTH = 1024;
    constexpr int SCREEN_HEIGHT = 768;
    constexpr float WORLD_WIDTH = SCREEN_WIDTH * 2.0f;
    constexpr float WORLD_HEIGHT = SCREEN_HEIGHT * 2.0f;
}
