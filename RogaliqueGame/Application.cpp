#include "Application.h"
#include "Player.h"
#include "Menu.h"
#include "SoundSettingsWindow.h"
#include "GameWorld.h"
#include "CameraComponent.h"
#include "TransformComponent.h"
#include "SpriteComponent.h"
#include "SoundManager.h"
#include <iostream>
#include <vector>

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
        
        // Загружаем звуки и музыку
        auto& sm = SoundManager::GetInstance();
        sm.LoadSound("logo", "D:/xyz/roqalique/RogaliqueGame/Resources/Sounds/logo.WAV");
        sm.LoadSound("click", "D:/xyz/roqalique/RogaliqueGame/Resources/Sounds/clik.WAV");
        sm.LoadSound("chest", "D:/xyz/roqalique/RogaliqueGame/Resources/Sounds/chest.WAV");
        
        // Загружаем атмосферы
        sm.LoadMusic("atmosphere_eerie", "D:/xyz/roqalique/RogaliqueGame/Resources/Sounds/atmosphere_eerie.WAV");
        sm.LoadMusic("atmosphere_dark", "D:/xyz/roqalique/RogaliqueGame/Resources/Sounds/atmosphere_dark.WAV");
        
        // Фоновая музыка меню
        sm.PlayMusicFile("D:/xyz/roqalique/RogaliqueGame/Resources/Sounds/main(1).WAV");
        
        std::cout << "[App] Application ready" << std::endl;
    }

    Application::~Application()
    {
        g_Application = nullptr;
        if (m_player)
            GameWorld::GetInstance().Clear();
        
        // Останавливаем музыку при выходе
        SoundManager::GetInstance().StopMusic();
        
        std::cout << "[App] Application destroyed" << std::endl;
    }

    void Application::Run()
    {
        std::cout << "[App] Run() started" << std::endl;
        
        ShowLogoSplash();
        
        std::cout << "[App] Logo splash finished" << std::endl;
        
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
                {
                    m_soundSettings->HandleInput(event);
                }
                else if (m_inMenu)
                {
                    m_menu->HandleInput(event);
                }
                else
                {
                    // Обработка Escape во время игры
                    if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
                    {
                        SoundManager::GetInstance().PlaySound("click");
                        ReturnToMenu();
                    }
                }
            }
            
            // Обработка окон
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
                {
                    StartGame();
                }
                else if (m_menu->IsSoundSelected())
                {
                    std::cout << "[App] Opening sound settings" << std::endl;
                    m_inMenu = false;
                    m_inSoundSettings = true;
                    m_soundSettings->Reset();
                }
                else if (m_menu->IsExitSelected())
                {
                    window.close();
                }
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
        if (m_camera && m_useCamera)
            m_camera->Update(deltaTime);
        
        auto& world = GameWorld::GetInstance();
        world.Update(deltaTime);
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
        
        window.display();
    }

    void Application::StartGame()
    {
        std::cout << "[App] Starting new game..." << std::endl;
        
        GameWorld::GetInstance().Clear();
        
        m_player = GameWorld::GetInstance().CreateGameObject<Player>();
        GameWorld::GetInstance().SpawnChests(10, WORLD_WIDTH, WORLD_HEIGHT);
        
        auto* transform = m_player->GetComponent<TransformComponent>();
        if (transform)
            transform->SetPosition(sf::Vector2f(WORLD_WIDTH / 2.0f, WORLD_HEIGHT / 2.0f));
        
        if (WORLD_WIDTH > SCREEN_WIDTH || WORLD_HEIGHT > SCREEN_HEIGHT)
        {
            m_useCamera = true;
            std::cout << "[App] Camera enabled" << std::endl;
            
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
            std::cout << "[App] Camera disabled" << std::endl;
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
        
        // Возвращаем фоновую музыку меню
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
}
