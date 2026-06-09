#include "Application.h"
#include "Player.h"
#include "Menu.h"
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
#include <iostream>
#include <vector>
#include <algorithm>

namespace rogalique
{
    Application* g_Application = nullptr;

    Application::Application()
        : window(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "Rogalique Game")
    {
        window.setFramerateLimit(60);
        g_Application = this;
        
        m_menu = std::make_unique<Menu>();
        m_soundSettings = std::make_unique<SoundSettingsWindow>();
        
        auto& sm = SoundManager::GetInstance();
        sm.LoadSound("logo", "D:/xyz/roqalique/RogaliqueGame/Resources/Sounds/logo.WAV");
        sm.LoadSound("click", "D:/xyz/roqalique/RogaliqueGame/Resources/Sounds/clik.WAV");
        sm.LoadSound("chest", "D:/xyz/roqalique/RogaliqueGame/Resources/Sounds/chest.WAV");
        sm.LoadMusic("atmosphere_eerie", "D:/xyz/roqalique/RogaliqueGame/Resources/Sounds/atmosphere_eerie.WAV");
        sm.LoadMusic("atmosphere_dark", "D:/xyz/roqalique/RogaliqueGame/Resources/Sounds/atmosphere_dark.WAV");
        sm.LoadMusic("atmosphere_trepidation", "D:/xyz/roqalique/RogaliqueGame/Resources/Sounds/trepidation.WAV");
        sm.PlayMusicFile("D:/xyz/roqalique/RogaliqueGame/Resources/Sounds/main(1).WAV");
        
        if (!m_uiFont.loadFromFile("D:/xyz/roqalique/RogaliqueGame/Resources/Fonts/Roboto-Regular.ttf"))
        {
            std::cout << "[App] Warning: Could not load UI font" << std::endl;
        }
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
        
        // Стрельба по F
        static bool fPressed = false;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::F)) {
        }
        else {
            fPressed = false;
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
        if (m_useCamera && m_camera)
            window.setView(m_gameView);
        else
            window.setView(window.getDefaultView());
            
        window.clear(sf::Color(20, 20, 40));
        
        auto& world = GameWorld::GetInstance();
        world.Render(window);
        
        RenderWeapons(window);
        
       

        
        
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
        
        GameWorld::GetInstance().Clear();
        
        m_gold = 0;
        m_chestsCollected = 0;
        UpdateUI();
        
        m_player = GameWorld::GetInstance().CreateGameObject<Player>();
        GameWorld::GetInstance().SetPlayer(m_player);
        
        BlockBuilder::LoadLevel("D:/xyz/roqalique/RogaliqueGame/Resources/level1.txt", WORLD_WIDTH, WORLD_HEIGHT, 40);
        
        GameWorld::GetInstance().SpawnChests(10, WORLD_WIDTH, WORLD_HEIGHT);
        
        auto* transform = m_player->GetComponent<TransformComponent>();
        if (transform)
            transform->SetPosition(sf::Vector2f(WORLD_WIDTH / 2.0f, WORLD_HEIGHT / 2.0f));
        
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
        std::cout << "[App] Game started!" << std::endl;
    }

    void Application::ReturnToMenu()
    {
        std::cout << "[App] ReturnToMenu() called" << std::endl;
        m_inMenu = true;
        m_menu->Reset();
        m_camera.reset();
        m_useCamera = false;
        m_player = nullptr;
        GameWorld::GetInstance().Clear();
        window.setView(window.getDefaultView());
        SoundManager::GetInstance().PlayMusicFile("D:/xyz/roqalique/RogaliqueGame/Resources/Sounds/main(1).WAV");
    }

    void Application::ShowLogoSplash()
    {
        SoundManager::GetInstance().PlaySound("logo");
        
        sf::Texture logoTexture;
        std::vector<std::string> logoPaths = {
            "D:/xyz/roqalique/RogaliqueGame/Resources/xyz-logo.png",
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
        std::cout << "[UI] UpdateUI called" << std::endl; 
        m_goldText.setString("Gold: " + std::to_string(m_gold));
        m_chestText.setString("Chests: " + std::to_string(m_chestsCollected));
        m_healthUI.Reset();
        if (m_player && m_player->GetWeapon()) {
            int ammo = m_player->GetWeapon()->GetCurrentAmmo();
            int maxAmmo = m_player->GetWeapon()->GetMaxAmmo();
            m_ammoText.setString("Ammo: " + std::to_string(ammo) + " / " + std::to_string(maxAmmo));
            std::cout << "[UI] Ammo: " << ammo << "/" << maxAmmo << std::endl;  
        }
        else {
            m_ammoText.setString("Ammo: 0 / 0");
            std::cout << "[UI] No weapon!" << std::endl;  
        }
    }
    
    void Application::SpawnWeapon(float x, float z) {
        WeaponItem* weapon = new WeaponItem(x, z);
        m_weaponItems.push_back(weapon);
        std::cout << "[Application] Weapon spawned at (" << x << ", " << z << ")" << std::endl;
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
            }
        }
    }
    
    void Application::RenderWeapons(sf::RenderWindow& window) {
        for (auto* weapon : m_weaponItems) {
            weapon->Render(window);
        }
    }
}
