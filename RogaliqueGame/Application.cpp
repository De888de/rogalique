#include "Application.h"
#include "Player.h"
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

        // Инициализация игрока
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
        // === ЗАСТАВКА С ЛОГОТИПОМ ===
        ShowLogoSplash();

        // === ИГРОВОЙ ЦИКЛ ===
        sf::Clock clock;

        while (window.isOpen())
        {
            float deltaTime = clock.restart().asSeconds();

            // Обработка событий
            sf::Event event;
            while (window.pollEvent(event))
            {
                if (event.type == sf::Event::Closed)
                    window.close();

                // Обработка нажатия Escape для выхода
                if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
                    window.close();
            }

            // Обновление игровых объектов
            Update(deltaTime);

            // Отрисовка
            Draw();
        }
    }

    void Application::Update(float deltaTime)
    {
        if (m_player)
            m_player->Update(deltaTime);

        // TODO: обновление врагов, пуль, эффектов
    }

    void Application::Draw()
    {
        window.clear(sf::Color(20, 20, 40));  // Тёмно-синий фон

        if (m_player)
            m_player->Draw(window);

        // TODO: отрисовка врагов, пуль, эффектов, UI

        window.display();
    }

    void Application::StartGame()
    {
        std::cout << "[App] StartGame() called" << std::endl;
        // TODO: инициализация новой игры
        if (m_player)
        {
            m_player->SetPosition(sf::Vector2f(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f));
            m_player->Heal(100);  // полное исцеление
        }
    }

    void Application::ReturnToMenu()
    {
        std::cout << "[App] ReturnToMenu() called" << std::endl;
        // TODO: возврат в главное меню
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