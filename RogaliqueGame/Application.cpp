#include "Application.h"
#include "Player.h"              // ← обязательно полный заголовок
#include "Menu.h"
#include "SoundSettingsWindow.h"
#include "GameWorld.h"
#include "CameraComponent.h"
#include "TransformComponent.h"
#include "SoundManager.h"
#include "BlockBuilder.h"
#include "PhysicsTestObject.h"
#include "IsometricPhysicsItem.h"
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
        
        UpdateUI();
        
        std::cout << "[App] Application ready" << std::endl;
    }

    Application::~Application()
    {
        g_Application = nullptr;
        if (m_player)
            GameWorld::GetInstance().Clear();
        SoundManager::GetInstance().StopMusic();
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
    
     // Спавн изометрического предмета по клавише I
     static bool iPressed = false;
     if (sf::Keyboard::isKeyPressed(sf::Keyboard::I))
     {
         if (!iPressed)
         {
             iPressed = true;
             auto* item = GameWorld::GetInstance().CreateGameObject<IsometricPhysicsItem>();
             if (item)
             {
                 item->SpawnInFrontOfPlayer(65.0f);   // удобно тестировать
             }
         }
     }
     else
     {
         iPressed = false;
     }
        
        // ===== ОБРАБОТКА КЛАВИШ ДЛЯ ТЕСТА ФИЗИКИ =====
        static bool pPressed = false;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::P)) {
            if (!pPressed) {
                pPressed = true;
                auto* testObj = GameWorld::GetInstance().CreateGameObject<PhysicsTestObject>();
                if (testObj) {
                    // Спавним перед игроком
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
        if (m_useCamera && m_camera)
            window.setView(m_gameView);
        else
            window.setView(window.getDefaultView());
            
        window.clear(sf::Color(20, 20, 40));
        
        auto& world = GameWorld::GetInstance();
        world.Render(window);
        
        sf::View previousView = window.getView();
        window.setView(window.getDefaultView());
        
        window.draw(m_goldText);
        window.draw(m_chestText);
        
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
        
        // Сначала игрок
        m_player = GameWorld::GetInstance().CreateGameObject<Player>();
        
        // Потом стены и враги
        BlockBuilder::LoadLevel("D:/xyz/roqalique/RogaliqueGame/Resources/level1.txt", WORLD_WIDTH, WORLD_HEIGHT, 40);
        
        // Потом сундуки
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
        m_goldText.setString("Gold: " + std::to_string(m_gold));
        m_chestText.setString("Chests: " + std::to_string(m_chestsCollected));
    }
}
