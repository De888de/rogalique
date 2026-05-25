#pragma once
#include <string>

namespace rogalique
{
    class GameSettings
    {
    private:
        GameSettings() = default;

    public:
        static GameSettings& Instance()
        {
            static GameSettings instance;
            return instance;
        }

        // Размер окна
        static constexpr float SCREEN_WIDTH = 1024.0f;
        static constexpr float SCREEN_HEIGHT = 768.0f;

        // Игрок
        static constexpr float PLAYER_SIZE = 32.0f;
        static constexpr float PLAYER_SPEED = 200.0f;
        static constexpr int PLAYER_MAX_HEALTH = 100;

        // Враги (базовые)
        static constexpr float ENEMY_SIZE = 32.0f;
        static constexpr float ENEMY_BASE_SPEED = 80.0f;

        // Пути к ресурсам
        static inline const char* TEXTURES_PATH = "Resources/textures/";
        static inline const char* FONTS_PATH = "Resources/Fonts/";
        static inline const char* SOUNDS_PATH = "Resources/sounds/";
    };
}

#define SETTINGS rogalique::GameSettings::Instance()
