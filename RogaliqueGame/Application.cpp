#include "Application.h"
#include "Player.h"
#include "Menu.h"
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
        
        m_player = std::make_unique<Player>();
        
        auto* transform = m_player->GetComponent<TransformComponent>();
        if (transform)
            transform->SetPosition(sf::Vector2f(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f));
        
        m_menu = std::make_unique<Menu>();
        
        std::cout << "[App] Application ready" << std::endl;
    }

    Application::~Application()
    {
        g_Application = nullptr;
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
                    m_inMenu = false;
                    std::cout << "[App] Starting game..." << std::endl;
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
        if (m_player)
            m_player->Update(deltaTime);
    }

    void Application::Draw()
    {
        window.clear(sf::Color(20, 20, 40));
        
        if (m_player)
            m_player->Render(window);
        
        window.display();
    }

    void Application::StartGame()
    {
        std::cout << "[App] StartGame() called" << std::endl;
        m_inMenu = false;
    }

    void Application::ReturnToMenu()
    {
        std::cout << "[App] ReturnToMenu() called" << std::endl;
        m_inMenu = true;
        m_menu->Reset();
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
