#include "Application.h"
#include <iostream>

namespace RoqaliqueGame {

    Application* g_Application = nullptr;

    Application::Application()
        : window(sf::VideoMode(1024, 768), "Rogalique Game")
    {
        window.setFramerateLimit(60);
        g_Application = this;
        std::cout << "[App] Application ready" << std::endl;
    }

    Application::~Application()
    {
        g_Application = nullptr;
    }

    void Application::Run()
    {
        // === ЗАСТАВКА С ЛОГОТИПОМ ===
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
            // Масштабируем
            float scaleX = 1024.0f / logoTexture.getSize().x;
            float scaleY = 768.0f / logoTexture.getSize().y;
            float scale = std::min(scaleX, scaleY) * 0.6f;
            logoSprite.setScale(scale, scale);

            // Центрируем
            logoSprite.setOrigin(logoTexture.getSize().x / 2.f, logoTexture.getSize().y / 2.f);
            logoSprite.setPosition(512.f, 384.f);

            window.clear(sf::Color::Black);
            window.draw(logoSprite);
            window.display();

            // Ждём 2 секунды
            sf::sleep(sf::seconds(2.0f));
        }
        else
        {
            std::cout << "[App] Warning: Could not load logo" << std::endl;
        }

        // === ОСНОВНОЙ ИГРОВОЙ ЦИКЛ ===
        while (window.isOpen())
        {
            sf::Event event;
            while (window.pollEvent(event))
            {
                if (event.type == sf::Event::Closed)
                    window.close();
            }

            window.clear(sf::Color::Blue);
            window.display();
        }
    }

    void Application::StartGame()
    {
        std::cout << "[App] StartGame() called" << std::endl;
    }

    void Application::ReturnToMenu()
    {
        std::cout << "[App] ReturnToMenu() called" << std::endl;
    }
}