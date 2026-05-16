#pragma once

#include "Block.h"
#include "ToughBlock.h"
#include "MagicBlock.h"
#include "GlassBlock.h"
#include "SmoothDestroyableBlock.h"
#include "IndestructibleBlock.h"

#include <vector>
#include <string>
#include <memory>
#include <map>

namespace RoqaliqueGame {

    enum class BlockType
    {
        Empty = 0,
        Simple,          // 1 - обычный блок
        ThreeHit,        // 2 - блок на 3 удара (ToughBlock)
        Indestructible,  // 3 - неуничтожимый блок
        Magic,           // 4 - магический блок (взрывает соседей)
        Glass,           // 5 - стеклянный блок (мяч пролетает)
        SmoothDestroy    // 6 - плавно исчезающий блок
    };

    struct BlockInfo
    {
        sf::Vector2i position;  // строка и колонка
        BlockType type;
    };

    struct Level
    {
        std::string name;
        std::vector<BlockInfo> blocks;
        int width = 0;   // ширина уровня в блоках
        int height = 0;  // высота уровня в блоках
    };

    class LevelLoader final
    {
    public:
        LevelLoader() { LoadLevelsFromFile(); }
        Level& GetLevel(int index);
        int GetLevelCount() const { return static_cast<int>(levels.size()); }
        void ReloadLevels() { LoadLevelsFromFile(); }

    private:
        void LoadLevelsFromFile();
        BlockType CharToBlockType(char symbol);
        bool ParseLevel(std::ifstream& file, Level& level);

        std::vector<Level> levels;
    };
}

