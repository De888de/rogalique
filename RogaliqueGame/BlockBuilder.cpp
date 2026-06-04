#include "BlockBuilder.h"
#include "GameWorld.h"
#include "TransformComponent.h"
#include "SpriteComponent.h"
#include "CollisionComponent.h"
#include "BlockComponent.h"
#include "Enemy.h"
#include <fstream>
#include <vector>
#include <iostream>

namespace rogalique
{
    void BlockBuilder::LoadLevel(const std::string& filename, float worldWidth, float worldHeight, float blockSize)
    {
        std::ifstream file(filename);
        if (!file.is_open())
        {
            std::cout << "[BlockBuilder] Failed to load level: " << filename << std::endl;
            return;
        }
        
        std::vector<std::string> map;
        std::string line;
        while (std::getline(file, line))
        {
            map.push_back(line);
        }
        
        float startX = (worldWidth - map[0].size() * blockSize) / 2.0f;
        float startY = (worldHeight - map.size() * blockSize) / 2.0f;
        
        auto& world = GameWorld::GetInstance();
        
        for (size_t row = 0; row < map.size(); ++row)
        {
            for (size_t col = 0; col < map[row].size(); ++col)
            {
                char c = map[row][col];
                float x = startX + col * blockSize + blockSize / 2.0f;
                float y = startY + row * blockSize + blockSize / 2.0f;
                
                if (c == '#')
                {
                    // Стена
                    auto* block = world.CreateGameObject<RogaliqueGameObject>();
                    auto* transform = block->AddComponent<TransformComponent>();
                    transform->SetPosition(sf::Vector2f(x, y));
                    
                    auto* sprite = block->AddComponent<SpriteComponent>("", blockSize, blockSize);
                    sprite->SetFallbackColor(sf::Color(180, 80, 40));
                    
                    auto* collision = block->AddComponent<CollisionComponent>(blockSize / 2.0f);
                    collision->SetTrigger(false);
                    
                    block->AddComponent<BlockComponent>();
                }
                else if (c == 'E')
                {
                    // Враг
                    auto* enemy = world.CreateGameObject<Enemy>();
                    auto* transform = enemy->GetComponent<TransformComponent>();
                    if (transform)
                        transform->SetPosition(sf::Vector2f(x, y));
                    std::cout << "[BlockBuilder] Spawned enemy at (" << x << ", " << y << ")" << std::endl;
                }
                // '.' — ничего не делаем
            }
        }
        
        std::cout << "[BlockBuilder] Loaded level: " << filename << std::endl;
    }
}
