#include "LevelLoader.h"
#include "GameSettings.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <cctype>

namespace RoqaliqueGame
{
    BlockType LevelLoader::CharToBlockType(char symbol)
    {
        switch (symbol)
        {
        case '1': return BlockType::Simple;
        case '2': case 'T': return BlockType::ThreeHit;
        case '3': return BlockType::Indestructible;
        case '4': case 'M': return BlockType::Magic;
        case '5': case 'G': return BlockType::Glass;
        case '6': case 'S': return BlockType::SmoothDestroy;
        case '0': default: return BlockType::Empty;
        }
    }

    void LevelLoader::LoadLevelsFromFile()
    {
        levels.clear();

        // Пробуем разные пути
        std::vector<std::string> paths = {
            "Resources/levels/levels.txt",
            "../Resources/levels/levels.txt",
            "../../Resources/levels/levels.txt",
            "levels.txt"
        };

        std::ifstream file;
        std::string loadedPath;

        for (const auto& path : paths)
        {
            file.open(path);
            if (file.is_open())
            {
                loadedPath = path;
                std::cout << "[LevelLoader] Found levels file: " << path << std::endl;
                break;
            }
        }

        if (!file.is_open())
        {
            std::cout << "[LevelLoader] ERROR: Could not find levels file!" << std::endl;
            return;
        }

        std::string line;

        while (std::getline(file, line))
        {
            // Удаляем пробелы
            line.erase(0, line.find_first_not_of(" \t\r\n"));
            line.erase(line.find_last_not_of(" \t\r\n") + 1);

            if (line.empty() || line[0] == ';' || line[0] == '/') continue;

            // Начало нового уровня
            if (line[0] == '[' && line.find(']') != std::string::npos)
            {
                Level newLevel;
                newLevel.name = line.substr(1, line.find(']') - 1);

                int maxCols = 0;
                int row = 0;
                bool readingMap = false;

                while (std::getline(file, line))
                {
                    line.erase(0, line.find_first_not_of(" \t\r\n"));
                    line.erase(line.find_last_not_of(" \t\r\n") + 1);

                    if (line.empty()) continue;
                    if (line[0] == '[') break;

                    std::string lowerLine = line;
                    std::transform(lowerLine.begin(), lowerLine.end(), lowerLine.begin(), ::tolower);

                    if (lowerLine.find("name=") == 0 && !readingMap)
                    {
                        size_t pos = line.find('=');
                        if (pos != std::string::npos)
                            newLevel.name = line.substr(pos + 1);
                    }
                    else if (lowerLine == "map:")
                    {
                        readingMap = true;
                        continue;
                    }

                    if (readingMap)
                    {
                        int col = 0;
                        for (char symbol : line)
                        {
                            if (symbol == '#' || symbol == ' ') continue;

                            BlockType type = CharToBlockType(symbol);
                            if (type != BlockType::Empty)
                            {
                                BlockInfo info;
                                info.position = sf::Vector2i(row, col);
                                info.type = type;
                                newLevel.blocks.push_back(info);
                            }
                            col++;
                        }
                        if (col > maxCols) maxCols = col;
                        row++;
                    }
                }

                newLevel.width = maxCols;
                newLevel.height = row;

                if (!newLevel.blocks.empty())
                {
                    levels.push_back(std::move(newLevel));
                    std::cout << "[LevelLoader] Loaded level " << levels.size()
                        << " ('" << levels.back().name << "') with "
                        << levels.back().blocks.size() << " blocks, "
                        << "width=" << levels.back().width << ", height=" << levels.back().height << std::endl;
                }
            }
        }

        file.close();
        std::cout << "[LevelLoader] Total levels loaded: " << levels.size() << std::endl;
    }

    Level& LevelLoader::GetLevel(int index)
    {
        if (index >= 0 && index < static_cast<int>(levels.size()))
        {
            return levels[index];
        }
        static Level emptyLevel;
        if (levels.empty()) return emptyLevel;
        return levels[0];
    }
}