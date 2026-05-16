#pragma once
#include <SFML/Graphics.hpp>
#include <memory>

namespace ArkanoidGame {
    class Menu;
    class Game;
}

namespace ArkanoidGame {

    class Application {
    public:
        Application();
        ~Application();

        void Run();
        void StartGame();
        void ReturnToMenu();

        sf::RenderWindow window;

    private:
        void HandleEvents();
        void Update(float timeDelta);
        void Draw();

        std::unique_ptr<Menu> menu_;
        std::unique_ptr<Game> game_;

        enum class AppState { MainMenu, Playing };
        AppState currentState_ = AppState::MainMenu;
    };

    extern Application* g_Application;

} // namespace ArkanoidGame