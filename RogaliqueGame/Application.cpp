#include "Application.h"
#include "Player.h"
#include "Menu.h"
#include "Logger.h"
#include "SoundSettingsWindow.h"
#include "GameWorld.h"
#include "CameraComponent.h"
#include "TransformComponent.h"
#include "SoundManager.h"
#include "BlockBuilder.h"
#include "PhysicsTestObject.h"
#include "IsometricPhysicsItem.h"
#include "WeaponItem.h"
#include "Bullet.h"
#include "Enemy.h"  
#include "HealthUI.h"
#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <cstdlib>
#include <ctime>

namespace rogalique
{
    Application* g_Application = nullptr;

    Application::Application()
        : window(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "Rogalique Game")
        , m_gameOver(false)
    {
        window.setFramerateLimit(60);
        g_Application = this;
        
        m_menu = std::make_unique<Menu>();
        m_soundSettings = std::make_unique<SoundSettingsWindow>();
        
        auto& sm = SoundManager::GetInstance();
        sm.LoadSound("logo", "RogaliqueGame/Resources/Sounds/logo.WAV");
        sm.LoadSound("click", "RogaliqueGame/Resources/Sounds/clik.WAV");
        sm.LoadSound("chest", "RogaliqueGame/Resources/Sounds/chest.WAV");
        sm.LoadMusic("atmosphere_eerie", "RogaliqueGame/Resources/Sounds/atmosphere_eerie.WAV");
        sm.LoadMusic("atmosphere_dark", "RogaliqueGame/Resources/Sounds/atmosphere_dark.WAV");
        sm.LoadMusic("atmosphere_trepidation", "RogaliqueGame/Resources/Sounds/trepidation.WAV");
        sm.LoadSound("gameover", "RogaliqueGame/Resources/Sounds/GAMEOVER.WAV");
        sm.LoadSound("death_scream", "RogaliqueGame/Resources/Sounds/hukc.WAV");
        sm.LoadSound("monster_hit", "RogaliqueGame/Resources/Sounds/monstr.WAV");
        sm.LoadSound("shot", "RogaliqueGame/Resources/Sounds/shot.WAV");
        sm.LoadMusic("dungeon", "RogaliqueGame/Resources/Sounds/dungeon.WAV");
        sm.LoadSound("attack_01", "RogaliqueGame/Resources/Sounds/attack_01.WAV");
        sm.LoadSound("attack_02", "RogaliqueGame/Resources/Sounds/attack_02.WAV");
        sm.LoadSound("metal_hit", "RogaliqueGame/Resources/Sounds/metal_hit.WAV");
        sm.LoadSound("hit", "RogaliqueGame/Resources/Sounds/hit.WAV");
        sm.PlayMusicFile("RogaliqueGame/Resources/Sounds/main(1).WAV");
        
        if (!m_uiFont.loadFromFile("RogaliqueGame/Resources/Fonts/Roboto-Regular.ttf"))
        {
            std::cout << "[App] Warning: Could not load UI font" << std::endl;
        }
        
        // UI тексты
        m_goldText.setFont(m_uiFont);
        m_goldText.setCharacterSize(24);
        m_goldText.setFillColor(sf::Color::Yellow);
        m_goldText.setPosition(20, 20);
        
        m_chestText.setFont(m_uiFont);
        m_chestText.setCharacterSize(24);
        m_chestText.setFillColor(sf::Color::Yellow);
        m_chestText.setPosition(20, 50);
        
        m_ammoText.setFont(m_uiFont);
        m_ammoText.setCharacterSize(24);
        m_ammoText.setFillColor(sf::Color::White);
        m_ammoText.setPosition(20, 80);
        
        // Game Over тексты
        m_gameOverText.setFont(m_uiFont);
        m_gameOverText.setCharacterSize(72);
        m_gameOverText.setFillColor(sf::Color::Red);
        m_gameOverText.setString("GAME OVER");
        m_gameOverText.setOrigin(m_gameOverText.getLocalBounds().width / 2, m_gameOverText.getLocalBounds().height / 2);
        m_gameOverText.setPosition(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 - 60);
        
        m_restartText.setFont(m_uiFont);
        m_restartText.setCharacterSize(28);
        m_restartText.setFillColor(sf::Color::White);
        m_restartText.setString("Press [R] to Restart     Press [ESC] to Menu");
        m_restartText.setOrigin(m_restartText.getLocalBounds().width / 2, m_restartText.getLocalBounds().height / 2);
        m_restartText.setPosition(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 + 40);
        
        UpdateUI();
        SpawnWeapon(500, 400);
        
        std::cout << "[App] Application ready" << std::endl;
    }

    Application::~Application()
    {
        g_Application = nullptr;
        if (m_player)
            GameWorld::GetInstance().Clear();
        SoundManager::GetInstance().StopMusic();
        
        for (auto* weapon : m_weaponItems) {
            delete weapon;
        }
        m_weaponItems.clear();
    }

    void Application::Run()
    {
        std::cout << "[App] Run() started" << std::endl;
        
        ShowLogoSplash();
        
        sf::Clock clock;
        
        while (window.isOpen())
        {
            float deltaTime = clock.restart().asSeconds();
            
            sf::Event event;
            while (window.pollEvent(event))
            {
                if (event.type == sf::Event::Closed)
                    window.close();
                
                if (m_gameOver) {
                    if (event.type == sf::Event::KeyPressed) {
                        if (event.key.code == sf::Keyboard::R) {
                            std::cout << "[App] Restarting game..." << std::endl;
                            m_gameOver = false;
                            StartGame();
                        } else if (event.key.code == sf::Keyboard::Escape) {
                            std::cout << "[App] Returning to menu..." << std::endl;
                            m_gameOver = false;
                            ReturnToMenu();
                        }
                    }
                    continue;
                }
                
                if (m_inSoundSettings)
                    m_soundSettings->HandleInput(event);
                else if (m_inMenu)
                    m_menu->HandleInput(event);
                else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
                {
                    SoundManager::GetInstance().PlaySound("click");
                    ReturnToMenu();
                }
            }
            
            if (m_gameOver) {
                Draw();
                continue;
            }
            
            if (m_inSoundSettings)
            {
                m_soundSettings->Update(deltaTime);
                window.clear(sf::Color(20, 20, 40));
                m_soundSettings->Draw(window);
                window.display();
                
                if (!m_soundSettings->IsActive())
                {
                    m_inSoundSettings = false;
                    m_inMenu = true;
                    m_menu->Reset();
                }
            }
            else if (m_inMenu)
            {
                m_menu->Update(deltaTime);
                window.clear(sf::Color(20, 20, 40));
                m_menu->Draw(window);
                window.display();
                
                if (m_menu->IsPlaySelected())
                    StartGame();
                else if (m_menu->IsSoundSelected())
                {
                    m_inMenu = false;
                    m_inSoundSettings = true;
                    m_soundSettings->Reset();
                }
                else if (m_menu->IsExitSelected())
                    window.close();
            }
            else
            {
                Update(deltaTime);
                Draw();
            }
        }
        
        std::cout << "[App] Run() finished" << std::endl;
    }

    void Application::Update(float deltaTime)
    {
        if (m_gameOver) return;
        
        UpdateWeapons(deltaTime);
        
        static bool iPressed = false;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::I))
        {
            if (!iPressed)
            {
                iPressed = true;
                auto* item = GameWorld::GetInstance().CreateGameObject<IsometricPhysicsItem>();
                if (item)
                {
                    item->SpawnInFrontOfPlayer(65.0f);
                }
            }
        }
        else
        {
            iPressed = false;
        }
        
        static bool pPressed = false;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::P)) {
            if (!pPressed) {
                pPressed = true;
                auto* testObj = GameWorld::GetInstance().CreateGameObject<PhysicsTestObject>();
                if (testObj) {
                    sf::Vector2f playerPos(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f);
                    if (m_player) {
                        auto* transform = m_player->GetComponent<TransformComponent>();
                        if (transform) playerPos = transform->GetPosition();
                    }
                    testObj->Spawn(playerPos.x + 50, playerPos.y - 50);
                    std::cout << "[Physics] Test object spawned!" << std::endl;
                }
            }
        }
        else {
            pPressed = false;
        }
        
        if (m_camera && m_useCamera)
            m_camera->Update(deltaTime);
        
        auto& world = GameWorld::GetInstance();
        world.Update(deltaTime);
        world.CheckCollisions();
        world.LateUpdate();
    }

    void Application::Draw()
    {
        if (m_gameOver)
        {
            window.clear(sf::Color(0, 0, 0, 200));
            window.draw(m_gameOverText);
            window.draw(m_restartText);
            window.setView(window.getDefaultView());
            window.display();
            return;
        }
        
        if (m_useCamera && m_camera)
            window.setView(m_gameView);
        else
            window.setView(window.getDefaultView());
            
        window.clear(sf::Color(20, 20, 40));
        
        auto& world = GameWorld::GetInstance();
        world.Render(window);
        
        RenderWeapons(window);
        
        // Сброс вида для UI
        sf::View previousView = window.getView();
        window.setView(window.getDefaultView());
        
        window.draw(m_goldText);
        window.draw(m_chestText);
        window.draw(m_ammoText);
        m_healthUI.Render(window);
        
        window.setView(previousView);
        window.display();
    }

    void Application::StartGame()
    {
        std::cout << "[App] Starting new game..." << std::endl;


        LOG_EVENT("Game Started", "New game beginning");
        
        GameWorld::GetInstance().Clear();

        // Очищаем старые предметы
        for (auto* weapon : m_weaponItems) {
            delete weapon;
        }
        m_weaponItems.clear();
        
        m_gold = 0;
        m_chestsCollected = 0;
        
        m_player = GameWorld::GetInstance().CreateGameObject<Player>();
        GameWorld::GetInstance().SetPlayer(m_player);
        
        // Сброс UI здоровья
        m_healthUI.Reset();
        
        BlockBuilder::LoadLevel("RogaliqueGame/Resources/level1.txt", WORLD_WIDTH, WORLD_HEIGHT, 40);
        
        GameWorld::GetInstance().SpawnChests(10, WORLD_WIDTH, WORLD_HEIGHT);
        
        // Спавн врагов
        for (int i = 0; i < 3; i++) {
            float x = 300 + rand() % (int)(WORLD_WIDTH - 600);
            float y = 300 + rand() % (int)(WORLD_HEIGHT - 600);
            auto* enemy = GameWorld::GetInstance().CreateGameObject<Enemy>();
            auto* transform = enemy->GetComponent<TransformComponent>();
            if (transform) transform->SetPosition(sf::Vector2f(x, y));
        }
        
        auto* transform = m_player->GetComponent<TransformComponent>();
        if (transform)
            transform->SetPosition(sf::Vector2f(WORLD_WIDTH / 2.0f, WORLD_HEIGHT / 2.0f));
        
        UpdateUI();

        // Спавн оружия на землю
        SpawnWeapon(WORLD_WIDTH / 2 - 100, WORLD_HEIGHT / 2);
        SpawnWeapon(WORLD_WIDTH / 2 + 100, WORLD_HEIGHT / 2);
        
        if (WORLD_WIDTH > SCREEN_WIDTH || WORLD_HEIGHT > SCREEN_HEIGHT)
        {
            m_useCamera = true;
            m_gameView.reset(sf::FloatRect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT));
            m_gameView.setViewport(sf::FloatRect(0, 0, 1, 1));
            m_camera = std::make_unique<CameraComponent>(nullptr);
            m_camera->SetTarget(m_player);
            m_camera->SetBounds(WORLD_WIDTH, WORLD_HEIGHT);
            m_camera->SetViewport(m_gameView);
        }
        else
        {
            m_useCamera = false;
            m_camera.reset();
        }
        
        m_inMenu = false;
        m_gameOver = false;
        std::cout << "[App] Game started!" << std::endl;
    }

    void Application::ReturnToMenu()
    {
        std::cout << "[App] ReturnToMenu() called" << std::endl;
        m_inMenu = true;
        m_gameOver = false;
        m_menu->Reset();
        m_camera.reset();
        m_useCamera = false;
        m_player = nullptr;
        GameWorld::GetInstance().Clear();
        window.setView(window.getDefaultView());
        SoundManager::GetInstance().PlayMusicFile("RogaliqueGame/Resources/Sounds/main(1).WAV");
    }

    void Application::ShowGameOver()
    {
        std::cout << "[App] ShowGameOver() called!" << std::endl;
        m_gameOver = true;
        std::cout << "[App] GAME OVER! m_gameOver = " << m_gameOver << std::endl;
        std::cout << "[App] GAME OVER! m_gameOver = " << m_gameOver << std::endl;
        std::cout << "[App] ShowGameOver() called!" << std::endl;

        LOG_EVENT("Game Over", "Player died");

        SoundManager::GetInstance().PlaySound("gameover");
    }

    void Application::ShowLogoSplash()
    {
        SoundManager::GetInstance().PlaySound("logo");
        
        sf::Texture logoTexture;
        std::vector<std::string> logoPaths = {
            "RogaliqueGame/Resources/xyz-logo.png",
            "../RogaliqueGame/Resources/xyz-logo.png",
            "Resources/xyz-logo.png"
        };
        
        bool logoLoaded = false;
        for (const auto& path : logoPaths)
        {
            if (logoTexture.loadFromFile(path))
            {
                logoLoaded = true;
                std::cout << "[App] Logo loaded from: " << path << std::endl;
                break;
            }
        }
        
        if (logoLoaded)
        {
            sf::Sprite logoSprite(logoTexture);
            float scaleX = SCREEN_WIDTH / logoTexture.getSize().x;
            float scaleY = SCREEN_HEIGHT / logoTexture.getSize().y;
            float scale = std::min(scaleX, scaleY) * 0.6f;
            logoSprite.setScale(scale, scale);
            logoSprite.setOrigin(logoTexture.getSize().x / 2.f, logoTexture.getSize().y / 2.f);
            logoSprite.setPosition(SCREEN_WIDTH / 2.f, SCREEN_HEIGHT / 2.f);
            
            window.clear(sf::Color::Black);
            window.draw(logoSprite);
            window.display();
            sf::sleep(sf::seconds(2.0f));
        }
        else
        {
            std::cout << "[App] Warning: Could not load logo" << std::endl;
        }
    }
    
    void Application::AddGold(int amount)
    {
        m_gold += amount;
        UpdateUI();
        std::cout << "[App] Gold: " << m_gold << std::endl;
    }
    
    void Application::AddChest()
    {
        m_chestsCollected++;
        UpdateUI();
        std::cout << "[App] Chests collected: " << m_chestsCollected << std::endl;
    }
    
    void Application::UpdateUI()
    {
        m_goldText.setString("Gold: " + std::to_string(m_gold));
        m_chestText.setString("Chests: " + std::to_string(m_chestsCollected));
        if (m_player && m_player->GetWeapon()) {
            int ammo = m_player->GetWeapon()->GetCurrentAmmo();
            int maxAmmo = m_player->GetWeapon()->GetMaxAmmo();
            m_ammoText.setString("Ammo: " + std::to_string(ammo) + " / " + std::to_string(maxAmmo));
        } else {
            m_ammoText.setString("Ammo: 0 / 0");
        }
    }
    
    void Application::SpawnWeapon(float x, float z) {
        WeaponItem* weapon = new WeaponItem(x, z);
        m_weaponItems.push_back(weapon);
        std::cout << "[Application] Weapon spawned at (" << x << ", " << z << ")" << std::endl;

        LOG_WEAPON_PICKUP("WeaponItem spawned at (" + std::to_string(x) + ", " + std::to_string(z) + ")");
    }
    
    void Application::UpdateWeapons(float dt) {
        for (int i = 0; i < (int)m_weaponItems.size(); i++) {
            m_weaponItems[i]->Update(dt);
            
            if (m_player && m_weaponItems[i]->CheckPickup(m_player->GetPosition())) {
                m_player->EquipWeapon();
                delete m_weaponItems[i];
                m_weaponItems.erase(m_weaponItems.begin() + i);
                i--;
                std::cout << "[Application] Weapon picked up!" << std::endl;

                LOG_EVENT("Weapon Pickup", "Player equipped weapon");
            }
        }
    }
    
    void Application::RenderWeapons(sf::RenderWindow& window) {
        for (auto* weapon : m_weaponItems) {
            weapon->Render(window);
        }
    }
}
