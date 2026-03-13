#pragma once
#include <string>

namespace SnakeGame
{
    // Путь к ресурсам
    const std::string RESOURCES_PATH = "D:\\xyz\\HW_02\\xyz-cpp-course\\SnakeGame\\Resources\\Textures\\";
    const std::string FONTS_PATH = "D:\\xyz\\HW_02\\xyz-cpp-course\\SnakeGame\\Resources\\Fonts\\";
    const std::string SOUNDS_PATH = "D:\\xyz\\HW_02\\xyz-cpp-course\\SnakeGame\\Resources\\Sounds\\";

    // Размеры экрана
    const int SCREEN_WIDTH = 800;
    const int SCREEN_HEIGHT = 600;
    const int GRID_CELL_SIZE = 20;
    const int GRID_WIDTH = SCREEN_WIDTH / GRID_CELL_SIZE;   // 40
    const int GRID_HEIGHT = SCREEN_HEIGHT / GRID_CELL_SIZE; // 30

    // Базовые параметры игры
    const float INITIAL_SPEED = 5.0f;
    const int INITIAL_SNAKE_LENGTH = 3;
    const int POINTS_PER_APPLE = 10;

    // Режимы сложности
    enum class Difficulty
    {
        EASY,
        MEDIUM,
        HARD,
        INSANE,
        MAGICAL
    };

    // Структура настроек для каждого режима сложности
    struct DifficultySettings
    {
        float speed;
        int rockCount;
        bool hasPortals;
        bool hasMagicApples;
        bool hasPoisonApples;        // ← новое поле
        float magicAppleDuration;
        const char* name;
    };

    // Массив настроек для всех режимов сложности
    const DifficultySettings DIFFICULTY_SETTINGS[5] = {
     {4.0f, 0, false, false,  true,  0.0f, "EASY"},     
     {5.0f, 10, false, false, true, 0.0f, "MEDIUM"},
     {7.0f, 30, false, true,  true, 0.0f, "HARD"},
     {9.0f, 50, true,  true,  true,  0.0f, "INSANE"},
     {9.0f, 70, true,  true,  true,  0.0f, "MAGICAL"}
    };

    // Вспомогательная функция для получения настроек по сложности
    inline const DifficultySettings& GetDifficultySettings(Difficulty diff)
    {
        return DIFFICULTY_SETTINGS[static_cast<int>(diff)];
    }
}