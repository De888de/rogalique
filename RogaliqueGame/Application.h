#pragma once
#include <SFML/Graphics.hpp>
#include <memory>
#include <vector>
#include "Player.h"
#include "Logger.h"
#include "HealthUI.h"
#include "DungeonManager.h"

namespace rogalique
{
    class Player;
    class Menu;
    class CameraComponent;
    class SoundSettingsWindow;
    class WeaponItem;
    class Bullet;

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
        void SpawnWeapon(float x, float z);
        void UpdateUI();
        void UpdateHealthUI(int current, int max) { m_healthUI.UpdateHealth(current, max); }
        void UpdateArmorUI(int current, int max) { m_healthUI.UpdateArmor(current, max); }
        void ShowGameOver();
        bool IsGameOver() const { return m_gameOver; }


        sf::RenderWindow window;
        Player* GetPlayer() const { return m_player; }

    private:
        void ShowLogoSplash();
        void Update(float deltaTime);
        void Draw();
        void UpdateWeapons(float dt);
        void RenderWeapons(sf::RenderWindow& window);
        void PlayRandomAttackSound();

        Player* m_player = nullptr;
        std::unique_ptr<Menu> m_menu;
        std::unique_ptr<CameraComponent> m_camera;
        std::unique_ptr<SoundSettingsWindow> m_soundSettings;
        std::unique_ptr<DungeonManager> m_dungeonManager;
        std::vector<WeaponItem*> m_weaponItems;
        sf::View m_gameView;
        
        bool m_inMenu = true;
        bool m_inSoundSettings = false;
        bool m_useCamera = false;
        bool m_gameOver = false;

        int m_gold = 0;
        int m_chestsCollected = 0;
        sf::Font m_uiFont;
        sf::Text m_goldText;
        sf::Text m_chestText;
        sf::Text m_ammoText;
        sf::Text m_gameOverText;
        sf::Text m_restartText;
        
        HealthUI m_healthUI;
    };

    extern Application* g_Application;

    constexpr int SCREEN_WIDTH = 1024;
    constexpr int SCREEN_HEIGHT = 768;
    constexpr float WORLD_WIDTH = SCREEN_WIDTH * 3.0f;
    constexpr float WORLD_HEIGHT = SCREEN_HEIGHT * 3.0f;
}
