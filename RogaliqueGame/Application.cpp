#include "Application.h"
#include "Player.h"
#include "Menu.h"
#include "GameWorld.h"
#include "TransformComponent.h"
#include "SpriteComponent.h"
#include <iostream>

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
                if (m_player)
                {
                    auto& world = GameWorld::GetInstance();
                    world.Update(deltaTime);
                    world.LateUpdate();
                    
                    window.clear(sf::Color(20, 20, 40));
                    world.Render(window);
                    window.display();
                }
            }
        }
    }

    void Application::StartGame()
    {
        std::cout << "[App] Starting new game..." << std::endl;
        
        // Очищаем предыдущий мир, если есть
        GameWorld::GetInstance().Clear();
        
        // Создаём игрока
        m_player = GameWorld::GetInstance().CreateGameObject<Player>();
        
        auto* transform = m_player->GetComponent<TransformComponent>();
        if (transform)
            transform->SetPosition(sf::Vector2f(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f));
        
        m_inMenu = false;
        std::cout << "[App] Game started!" << std::endl;
    }

    void Application::ReturnToMenu()
    {
        std::cout << "[App] ReturnToMenu() called" << std::endl;
        m_inMenu = true;
        m_menu->Reset();
        m_player = nullptr;  // указатель уже очищен в GameWorld::Clear()
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
