#include "Application.h"
#include "GameSettings.h"
#include "Menu.h"
#include "Game.h"
#include <iostream>

namespace ArkanoidGame {

    extern Game* g_Game;
    Application* g_Application = nullptr;

    Application::Application()
        : window(sf::VideoMode(static_cast<unsigned>(SETTINGS.SCREEN_WIDTH),
            static_cast<unsigned>(SETTINGS.SCREEN_HEIGHT)), "Arkanoid")
    {
        window.setFramerateLimit(60);
        g_Application = this;

        // Сначала создаём Game, чтобы g_Game был доступен для Menu
        game_ = std::make_unique<Game>();
        menu_ = std::make_unique<Menu>();

        g_Game = game_.get();

        std::cout << "[App] Application ready" << std::endl;
    }

    Application::~Application()
    {
        g_Game = nullptr;
        g_Application = nullptr;
    }

    void Application::Run()
    {
        sf::Clock clock;

        while (window.isOpen())
        {
            float timeDelta = clock.restart().asSeconds();

            HandleEvents();

            if (currentState_ == AppState::Playing && game_)
            {
                // Игровое состояние - обновляем и рисуем игру
                game_->UpdateGame(timeDelta, window);
            }
            else if (currentState_ == AppState::MainMenu && menu_)
            {
                // Меню - обновляем и рисуем
                menu_->Update(timeDelta);
                Draw();
            }
        }
    }

    void Application::HandleEvents()
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
                return;
            }

            if (currentState_ == AppState::Playing && game_)
            {
                game_->HandleWindowEvent(event);
                if (!window.isOpen()) return;
            }
            else if (currentState_ == AppState::MainMenu && menu_)
            {
                menu_->HandleWindowEvent(event);
            }
        }
    }

    void Application::Update(float timeDelta)
    {
        // Обновление теперь происходит в Run() для каждого состояния отдельно
        // Этот метод оставлен для совместимости, но не используется
    }

    void Application::Draw()
    {
        if (currentState_ == AppState::MainMenu && menu_)
        {
            window.clear(sf::Color::Black);
            menu_->Draw(window);
            window.display();
        }
        // Для Playing состояние рисуется внутри game_->UpdateGame
    }

    void Application::StartGame()
    {
        std::cout << "[App] StartGame() called - switching to Playing state" << std::endl;
        currentState_ = AppState::Playing;
        // Важно: НЕ вызываем game_->StartGame() здесь!
        // Game::StartGame() или Game::ContinueGame() уже были вызваны из Menu
    }

    void Application::ReturnToMenu()
    {
        std::cout << "[App] ReturnToMenu() called!" << std::endl;
        currentState_ = AppState::MainMenu;

        // Обновляем состояние сохранения в меню
        if (menu_ && game_)
        {
            menu_->UpdateHasSave(game_->HasSavedProgress());
        }

        // Перерисовываем меню
        Draw();
    }

} // namespace ArkanoidGame