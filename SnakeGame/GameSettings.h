#pragma once
#include <string>

namespace ArkanoidGame
{
    struct GameSettings
    {
        static constexpr float SCREEN_WIDTH = 800.0f;
        static constexpr float SCREEN_HEIGHT = 600.0f;

        static constexpr float PLATFORM_WIDTH = 100.0f;
        static constexpr float PLATFORM_HEIGHT = 20.0f;
        static constexpr float PLATFORM_SPEED = 600.0f;

        static constexpr float BALL_SIZE = 16.0f;
        static constexpr float BALL_SPEED = 450.0f;

        static constexpr const char* TEXTURES_PATH = "Resources/textures/";
        static constexpr const char* FONTS_PATH = "Resources/Fonts/";
    };

    inline const GameSettings SETTINGS;
}
