#include "BlockFactory.h"
#include "GameSettings.h"
#include "ToughBlock.h"
#include "MagicBlock.h"
#include "GlassBlock.h"
#include "IndestructibleBlock.h"
#include "SmoothDestroyableBlock.h"
#include <random>
#include <chrono>
#include <iostream>

namespace ArkanoidGame {

    void BlockFactory::PlaceSpecialBlocks(int rows, int cols,
        int& outMagicRow, int& outMagicCol,
        std::vector<std::pair<int, int>>& outGlassPositions)
    {
        static std::mt19937 gen(static_cast<unsigned>(std::chrono::steady_clock::now().time_since_epoch().count()));

        std::uniform_int_distribution<> rowDist(1, rows - 1);
        std::uniform_int_distribution<> colDist(1, cols - 2);

        outMagicRow = rowDist(gen);
        outMagicCol = colDist(gen);

        // Стеклянные блоки — максимум 3
        outGlassPositions.clear();
        int attempts = 0;
        while (outGlassPositions.size() < 3 && attempts < 50) {
            int r = rowDist(gen);
            int c = colDist(gen);
            if (r == outMagicRow && c == outMagicCol) continue;

            bool exists = false;
            for (const auto& p : outGlassPositions) {
                if (p.first == r && p.second == c) { exists = true; break; }
            }
            if (!exists) outGlassPositions.emplace_back(r, c);
            attempts++;
        }
    }

    void BlockFactory::BuildStandardLevel(std::vector<std::unique_ptr<Block>>& blocks)
    {
        blocks.clear();

        const float blockW = SETTINGS.BLOCK_WIDTH;
        const float blockH = SETTINGS.BLOCK_HEIGHT;
        const float padding = SETTINGS.BLOCK_PADDING;
        const int rows = SETTINGS.BLOCK_ROWS;
        const int cols = SETTINGS.BLOCK_COLUMNS;

        float totalBlocksWidth = cols * blockW + (cols - 1) * padding;
        float startX = (SETTINGS.SCREEN_WIDTH - totalBlocksWidth) / 2.0f;
        float startY = SETTINGS.BLOCK_START_Y;

        SetGlobalBlocksPtr(&blocks);   // важно для MagicBlock

        int magicRow, magicCol;
        std::vector<std::pair<int, int>> glassPositions;
        PlaceSpecialBlocks(rows, cols, magicRow, magicCol, glassPositions);

        std::cout << "===== BLOCK CREATION (via BlockFactory) =====" << std::endl;
        std::cout << "[MagicBlock] Placed at row " << magicRow << ", col " << magicCol << std::endl;
        std::cout << "[GlassBlock] Count: " << glassPositions.size() << std::endl;

        for (int r = 0; r < rows; ++r) {
            for (int c = 0; c < cols; ++c) {
                float x = startX + c * (blockW + padding);
                float y = startY + r * (blockH + padding);
                sf::Vector2f center(x + blockW / 2.0f, y + blockH / 2.0f);

                std::unique_ptr<Block> block;

                // стеклянные
                bool isGlass = false;
                for (const auto& pos : glassPositions) {
                    if (r == pos.first && c == pos.second) { isGlass = true; break; }
                }

                if (isGlass) {
                    block = std::make_unique<GlassBlock>(center);
                }
                else if (c == 0 || c == cols - 1) {
                    block = std::make_unique<IndestructibleBlock>(center);
                }
                else if (r == magicRow && c == magicCol) {
                    block = std::make_unique<MagicBlock>(center);
                }
                else if (r == 0) {
                    block = std::make_unique<ToughBlock>(center);
                }
                else {
                    block = std::make_unique<SmoothDestroyableBlock>(center);
                }

                blocks.push_back(std::move(block));
            }
        }
    }

} // namespace ArkanoidGame
