#pragma once
#include "Block.h"
#include <vector>
#include <memory>

namespace ArkanoidGame {

    class BlockFactory {
    public:
        // Основной метод — строит уровень (пока только один, легко добавить Level 2, 3 и т.д.)
        static void BuildStandardLevel(std::vector<std::unique_ptr<Block>>& blocks);

        

    private:
        static void PlaceSpecialBlocks(int rows, int cols,
            int& outMagicRow, int& outMagicCol,
            std::vector<std::pair<int, int>>& outGlassPositions);
    };

} // namespace ArkanoidGame

