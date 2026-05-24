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
