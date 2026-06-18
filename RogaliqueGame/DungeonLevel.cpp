#include "DungeonLevel.h"
#include "GameWorld.h"
#include "Player.h"
#include "TransformComponent.h"
#include "HealthComponent.h"
#include "SoundManager.h"
#include <iostream>
#include <random>

namespace rogalique {

DungeonLevel::DungeonLevel() : m_maze(41, 41) {}

void DungeonLevel::Generate(int levelNumber, Player* player) {
    m_levelNumber = levelNumber;
    std::cout << "[DungeonLevel] Generating level " << levelNumber << std::endl;
    
    int size = 41 + (levelNumber - 1) * 4;
    m_maze = MazeGenerator(size, size);
    
    unsigned int seed = 12345 + levelNumber * 777;
    m_maze.Generate(seed);
    
    int roomCount = 5 + levelNumber * 2;
    m_maze.CarveRooms(roomCount);
    m_maze.RemoveDeadEnds(0.3f);
    
    SpawnWalls();
    
    auto rooms = m_maze.GetRooms();
    if (!rooms.empty()) {
        sf::Vector2i startRoom = rooms[0];
        m_playerStart = MazeToWorld(startRoom.x, startRoom.y);
    } else {
        m_playerStart = MazeToWorld(m_maze.GetWidth() / 2, m_maze.GetHeight() / 2);
    }
    
    if (rooms.size() > 1) {
        sf::Vector2i portalRoom = rooms[rooms.size() - 1];
        sf::Vector2f portalPos = MazeToWorld(portalRoom.x, portalRoom.y);
        m_portal.SetPosition(portalPos.x, portalPos.y);
        m_portal.Deactivate();
    }
    
    SpawnEnemies(levelNumber);
    m_allEnemiesDefeated = false;
}

void DungeonLevel::SpawnWalls() {
    int width = m_maze.GetWidth();
    int height = m_maze.GetHeight();
    
    int wallCount = 0;
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            // Используем IsWalkable для проверки
            if (!m_maze.IsWalkable(x, y)) {
                sf::Vector2f worldPos = MazeToWorld(x, y);
                Wall* wall = GameWorld::GetInstance().CreateGameObject<Wall>(
                    worldPos.x, worldPos.y, CELL_SIZE, CELL_SIZE
                );
                if (wall) {
                    m_walls.push_back(wall);
                    wallCount++;
                }
            }
        }
    }
    std::cout << "[DungeonLevel] Spawned " << wallCount << " walls" << std::endl;
}
void DungeonLevel::SpawnEnemies(int level) {
    int enemyCount = level * 2 + 3;
    auto rooms = m_maze.GetRooms();
    
    if (rooms.size() < 3) return;
    
    std::vector<int> availableRooms;
    for (int i = 1; i < (int)rooms.size() - 1; i++) {
        availableRooms.push_back(i);
    }
    
    std::random_device rd;
    std::mt19937 rng(rd());
    std::shuffle(availableRooms.begin(), availableRooms.end(), rng);
    
    int spawned = 0;
    for (int roomIdx : availableRooms) {
        if (spawned >= enemyCount) break;
        
        int enemiesInRoom = (rng() % 2) + 1;
        
        for (int j = 0; j < enemiesInRoom && spawned < enemyCount; j++) {
            sf::Vector2i roomCenter = rooms[roomIdx];
            
            int offsetX = (rng() % 20) - 10;
            int offsetY = (rng() % 20) - 10;
            sf::Vector2f worldPos = MazeToWorld(roomCenter.x + offsetX/32, roomCenter.y + offsetY/32);
            
            Enemy* enemy = GameWorld::GetInstance().CreateGameObject<Enemy>();
            auto* transform = enemy->GetComponent<TransformComponent>();
            if (transform) {
                transform->SetPosition(worldPos);
            }
            
            auto* health = enemy->GetComponent<HealthComponent>();
            if (health) {
                int maxHealth = 20 + level * 10;
                // health->SetMaxHealth(maxHealth); // FIXME: нет такого метода
                // health->SetHealth(maxHealth); // FIXME: нет такого метода
            }
            
            m_enemies.push_back(enemy);
            spawned++;
        }
    }
}

void DungeonLevel::Update(float deltaTime) {
    m_portal.Update(deltaTime);
    
    if (!m_allEnemiesDefeated) {
        CheckEnemiesDefeated();
    }
}

void DungeonLevel::CheckEnemiesDefeated() {
    if (m_allEnemiesDefeated) return;
    
    bool allDead = true;
    for (Enemy* enemy : m_enemies) {
        if (enemy->IsAlive()) {
            allDead = false;
            break;
        }
    }
    
    if (allDead) {
        m_allEnemiesDefeated = true;
        m_portal.Activate();
        std::cout << "[DungeonLevel] ALL ENEMIES DEFEATED! PORTAL OPENED!" << std::endl;
        SoundManager::GetInstance().PlaySound("chest");
    }
}

void DungeonLevel::Clear() {
    for (Enemy* enemy : m_enemies) {
        GameWorld::GetInstance().DestroyGameObject(enemy);
    }
    m_enemies.clear();
    
    for (Wall* wall : m_walls) {
        GameWorld::GetInstance().DestroyGameObject(wall);
    }
    m_walls.clear();
    
    m_allEnemiesDefeated = false;
    m_portal.Deactivate();
}

sf::Vector2f DungeonLevel::MazeToWorld(int mx, int my) const {
    return sf::Vector2f(
        mx * CELL_SIZE + CELL_SIZE / 2.0f,
        my * CELL_SIZE + CELL_SIZE / 2.0f
    );
}

sf::Vector2i DungeonLevel::WorldToMaze(float wx, float wy) const {
    return sf::Vector2i(
        static_cast<int>(wx / CELL_SIZE),
        static_cast<int>(wy / CELL_SIZE)
    );
}

}
