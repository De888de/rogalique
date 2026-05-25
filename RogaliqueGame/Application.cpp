#include "Application.h"
#include "Player.h"
#include "Menu.h"
#include "GameWorld.h"
#include "CameraComponent.h"
#include "TransformComponent.h"
#include "SpriteComponent.h"
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
        
        std::cout << "[App] Application ready" << std::endl;
    }

    Application::~Application()
    {
        g_Application = nullptr;
        if (m_player)
            GameWorld::GetInstance().Clear();
    }

    void Application::Run()
    {
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
                    
                if (m_inMenu)
                    m_menu->HandleInput(event);
            }
            
            if (m_inMenu)
            {
                m_menu->Update(deltaTime);
                window.clear(sf::Color(20, 20, 40));
                m_menu->Draw(window);
                window.display();
                
                if (m_menu->IsPlaySelected())
                {
                    StartGame();
                }
                if (m_menu->IsExitSelected())
                    window.close();
            }
            else
            {
                Update(deltaTime);
                Draw();
            }
        }
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
        
        // Очищаем предыдущий мир
        GameWorld::GetInstance().Clear();
        
        // Создаём игрока
        m_player = GameWorld::GetInstance().CreateGameObject<Player>();
        GameWorld::GetInstance().SpawnChests(10, WORLD_WIDTH, WORLD_HEIGHT);
        
        auto* transform = m_player->GetComponent<TransformComponent>();
        if (transform)
            transform->SetPosition(sf::Vector2f(WORLD_WIDTH / 2.0f, WORLD_HEIGHT / 2.0f));
        
        // Решаем, нужна ли камера
        if (WORLD_WIDTH > SCREEN_WIDTH || WORLD_HEIGHT > SCREEN_HEIGHT)
        {
            m_useCamera = true;
            std::cout << "[App] Camera enabled: world larger than screen" << std::endl;
            
            // Настраиваем камеру
            m_gameView.reset(sf::FloatRect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT));
            m_gameView.setViewport(sf::FloatRect(0, 0, 1, 1));
            
            // Создаём компонент камеры
            m_camera = std::make_unique<CameraComponent>(nullptr);
            m_camera->SetTarget(m_player);
            m_camera->SetBounds(WORLD_WIDTH, WORLD_HEIGHT);
            m_camera->SetViewport(m_gameView);
        }
        else
        {
            m_useCamera = false;
            m_camera.reset();
            std::cout << "[App] Camera disabled: world fits screen" << std::endl;
        }
        
        m_inMenu = false;
        std::cout << "[App] Game started! World size: " << WORLD_WIDTH << "x" << WORLD_HEIGHT 
                  << ", Camera: " << (m_useCamera ? "ON" : "OFF") << std::endl;
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
    }

    void Application::ShowLogoSplash()
    {
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
