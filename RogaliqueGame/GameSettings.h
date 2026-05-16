#pragma once
#include <string>

namespace RoqaliqueGame
{

    class GameWorld {
    private:
        GameWorld() = default;

    public:

        static GameWorld& Instance() {
            static GameWorld gameWorld;
            return gameWorld;
        }

    
    
        static constexpr float SCREEN_WIDTH = 800.0f;
        static constexpr float SCREEN_HEIGHT = 600.0f;

        static constexpr float PLATFORM_WIDTH = 100.0f;
        static constexpr float PLATFORM_HEIGHT = 20.0f;
        static constexpr float PLATFORM_SPEED = 600.0f;

        static constexpr float BALL_SIZE = 16.0f;
        static constexpr float BALL_SPEED = 450.0f;

        // ==================== НАСТРОЙКИ БЛОКОВ ====================
        static constexpr int   BLOCK_ROWS = 5;
        static constexpr int   BLOCK_COLUMNS = 8;

        static constexpr float BLOCK_WIDTH = 62.0f;
        static constexpr float BLOCK_HEIGHT = 24.0f;
        static constexpr float BLOCK_PADDING = 12.0f;
        static constexpr float BLOCK_START_Y = 85.0f;

        static constexpr int SCORE_PER_BLOCK = 10;

        
        static inline const char* TEXTURES_PATH = "Resources/textures/";
        static inline const char* FONTS_PATH = "Resources/Fonts/";
    };

    
}

#define SETTINGS GameWorld :: Instance()