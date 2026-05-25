#include "Application.h"
#include "Player.h"
#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>

namespace rogalique
{
    Application* g_Application = nullptr;

    Application::Application()
        : window(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "Rogalique Game")
    {
        window.setFramerateLimit(60);
        g_Application = this;
        
        m_player = std::make_unique<Player>();
        m_player->SetPosition(sf::Vector2f(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f));
        
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
                if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
                    window.close();
            }
            
            Update(deltaTime);
            Draw();
        }
    }

    void Application::Update(float deltaTime)
    {
        if (!m_player) return;
        
        sf::Vector2f move(0, 0);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) move.y -= 1;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) move.y += 1;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) move.x -= 1;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) move.x += 1;
        
        if (move.x != 0 || move.y != 0)
        {
            float len = std::sqrt(move.x * move.x + move.y * move.y);
            move /= len;
        }
        
        sf::Vector2f pos = m_player->GetPosition();
        pos += move * 200.0f * deltaTime;
        
        pos.x = std::clamp(pos.x, 20.0f, SCREEN_WIDTH - 20.0f);
        pos.y = std::clamp(pos.y, 20.0f, SCREEN_HEIGHT - 20.0f);
        
        m_player->SetPosition(pos);
    }

    void Application::Draw()
    {
        window.clear(sf::Color(20, 20, 40));
        
        if (m_player)
        {
            sf::RectangleShape rect(sf::Vector2f(32, 32));
            rect.setFillColor(sf::Color::Green);
            rect.setOrigin(16, 16);
            rect.setPosition(m_player->GetPosition());
            window.draw(rect);
        }
        
        window.display();
    }

    void Application::StartGame()
    {
        std::cout << "[App] StartGame() called" << std::endl;
        if (m_player)
            m_player->SetPosition(sf::Vector2f(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f));
    }

    void Application::ReturnToMenu()
    {
        std::cout << "[App] ReturnToMenu() called" << std::endl;
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
