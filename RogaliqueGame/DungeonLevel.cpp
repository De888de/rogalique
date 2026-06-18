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
    m_levelComplete = false;
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
    
    // Портал в последней комнате (пока неактивен)
    if (rooms.size() > 1) {
        sf::Vector2i portalRoom = rooms[rooms.size() - 1];
        sf::Vector2f portalPos = MazeToWorld(portalRoom.x, portalRoom.y);
        m_portal.SetPosition(portalPos.x, portalPos.y);
        m_portal.Deactivate();
        std::cout << "[DungeonLevel] Portal at (" << portalPos.x << ", " << portalPos.y << ")" << std::endl;
    }
    
    // Спавним сундук перехода (в случайной комнате, не в стартовой и не в портальной)
    SpawnExitChest();
    
    // Спавним врагов (для атмосферы, но они не обязательны для перехода)
    SpawnEnemies(levelNumber);
    
    std::cout << "[DungeonLevel] Level " << levelNumber << " generated!" << std::endl;
    std::cout << "[DungeonLevel] Find the golden chest to open the portal!" << std::endl;
}

void DungeonLevel::SpawnWalls() {
    int width = m_maze.GetWidth();
    int height = m_maze.GetHeight();
    
    int wallCount = 0;
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
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

void DungeonLevel::SpawnExitChest() {
    std::cout << "[DungeonLevel] SpawnExitChest called!" << std::endl;

    auto rooms = m_maze.GetRooms();
    if (rooms.empty()) {
        std::cout << "[DungeonLevel] No rooms!" << std::endl;
        return;
    }

    // Находим центральную комнату
    int centerX = m_maze.GetWidth() / 2;
    int centerY = m_maze.GetHeight() / 2;

    int bestRoomIdx = 0;
    float bestDist = 999999.0f;

    for (size_t i = 0; i < rooms.size(); i++) {
        float dx = rooms[i].x - centerX;
        float dy = rooms[i].y - centerY;
        float dist = dx * dx + dy * dy;
        if (dist < bestDist) {
            bestDist = dist;
            bestRoomIdx = i;
        }
    }

    sf::Vector2i roomCenter = rooms[bestRoomIdx];
    std::cout << "[DungeonLevel] Central room at: (" << roomCenter.x << ", " << roomCenter.y << ")" << std::endl;

    // ПРОСТО спавним в центре комнаты
    sf::Vector2f worldPos = MazeToWorld(roomCenter.x, roomCenter.y);
    std::cout << "[DungeonLevel] Spawning chest at world: (" << worldPos.x << ", " << worldPos.y << ")" << std::endl;

    m_exitChest = GameWorld::GetInstance().CreateGameObject<Chest>();
    if (m_exitChest) {
        auto* transform = m_exitChest->GetComponent<TransformComponent>();
        if (transform) {
            transform->SetPosition(worldPos);
        }
        std::cout << "[DungeonLevel] Exit chest spawned at CENTER of room!" << std::endl;
    }
    else {
        std::cout << "[DungeonLevel] Failed to create chest!" << std::endl;
    }
}
void DungeonLevel::SpawnEnemies(int level) {
    int enemyCount = level * 2 + 1; // Немного врагов для атмосферы
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
    std::cout << "[DungeonLevel] Room center: (" << roomCenter.x << ", " << roomCenter.y << ") IsWalkable: " << m_maze.IsWalkable(roomCenter.x, roomCenter.y) << std::endl;
            
            int offsetX = (rng() % 20) - 10;
            int offsetY = (rng() % 20) - 10;
            sf::Vector2f worldPos = MazeToWorld(roomCenter.x + offsetX/32, roomCenter.y + offsetY/32);
            
            Enemy* enemy = GameWorld::GetInstance().CreateGameObject<Enemy>();
            if (enemy) {
                auto* transform = enemy->GetComponent<TransformComponent>();
                if (transform) {
                    transform->SetPosition(worldPos);
                }
                m_enemies.push_back(enemy);
                spawned++;
            }
        }
    }
    std::cout << "[DungeonLevel] Spawned " << spawned << " enemies" << std::endl;
}

void DungeonLevel::Update(float deltaTime) {
    m_portal.Update(deltaTime);
    
    // Проверяем собран ли сундук
    if (!m_levelComplete) {
        CheckChestCollected();
    }
}

void DungeonLevel::CheckChestCollected() {
    if (m_levelComplete) return;
    if (!m_exitChest) return;
    
    // Проверяем собран ли сундук
    if (m_exitChest->IsCollected()) {
        m_levelComplete = true;
        m_portal.Activate();
        std::cout << "[DungeonLevel] 🎉 CHEST COLLECTED! PORTAL OPENED!" << std::endl;
        SoundManager::GetInstance().PlaySound("chest");
        
        // Удаляем сундук
        m_exitChest = nullptr;
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
    
    if (m_exitChest) {
        GameWorld::GetInstance().DestroyGameObject(m_exitChest);
        m_exitChest = nullptr;
    }
    
    m_levelComplete = false;
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
