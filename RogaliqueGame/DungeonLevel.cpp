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
    m_requiredChests = levelNumber;
    m_collectedChests = 0;
    m_chests.clear();
    
    std::cout << "[DungeonLevel] Generating level " << levelNumber << std::endl;
    std::cout << "[DungeonLevel] Need to find " << m_requiredChests << " chests!" << std::endl;
    
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
    
    SpawnChests();
    SpawnEnemies(levelNumber);
    
    std::cout << "[DungeonLevel] Level " << levelNumber << " generated!" << std::endl;
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

void DungeonLevel::SpawnChests() {
    std::cout << "[DungeonLevel] Spawning " << m_requiredChests << " chests..." << std::endl;
    
    std::vector<sf::Vector2i> walkableCells;
    auto rooms = m_maze.GetRooms();
    
    if (rooms.size() < 2) {
        std::cout << "[DungeonLevel] Not enough rooms!" << std::endl;
        return;
    }
    
    // Собираем все проходимые клетки из всех комнат (кроме первой и последней)
    for (int i = 1; i < (int)rooms.size() - 1; i++) {
        sf::Vector2i center = rooms[i];
        // Проверяем область 3x3 вокруг центра комнаты
        for (int dy = -2; dy <= 2; dy++) {
            for (int dx = -2; dx <= 2; dx++) {
                int mx = center.x + dx;
                int my = center.y + dy;
                if (m_maze.IsWalkable(mx, my)) {
                    // Проверяем что клетка не занята стеной
                    bool isWall = false;
                    for (int wy = -1; wy <= 1 && !isWall; wy++) {
                        for (int wx = -1; wx <= 1 && !isWall; wx++) {
                            if (!m_maze.IsWalkable(mx + wx, my + wy)) {
                                isWall = true;
                            }
                        }
                    }
                    if (!isWall) {
                        walkableCells.push_back({mx, my});
                    }
                }
            }
        }
    }
    
    // Если мало клеток - добавляем все проходимые клетки лабиринта
    if (walkableCells.size() < m_requiredChests) {
        std::cout << "[DungeonLevel] Not enough cells in rooms, searching whole maze..." << std::endl;
        for (int y = 1; y < m_maze.GetHeight() - 1; y++) {
            for (int x = 1; x < m_maze.GetWidth() - 1; x++) {
                if (m_maze.IsWalkable(x, y)) {
                    walkableCells.push_back({x, y});
                }
            }
        }
    }
    
    // Перемешиваем
    std::random_device rd;
    std::mt19937 rng(rd());
    std::shuffle(walkableCells.begin(), walkableCells.end(), rng);
    
    // Создаём сундуки
    int spawned = 0;
    for (auto& pos : walkableCells) {
        if (spawned >= m_requiredChests) break;
        
        sf::Vector2f worldPos = MazeToWorld(pos.x, pos.y);
        Chest* chest = GameWorld::GetInstance().CreateGameObject<Chest>();
        if (chest) {
            auto* transform = chest->GetComponent<TransformComponent>();
            if (transform) {
                transform->SetPosition(worldPos);
            }
            m_chests.push_back(chest);
            std::cout << "[DungeonLevel] Chest " << (spawned+1) << "/" << m_requiredChests 
                      << " spawned at (" << worldPos.x << ", " << worldPos.y << ")" << std::endl;
            spawned++;
        }
    }
    
    if (spawned < m_requiredChests) {
        std::cout << "[DungeonLevel] WARNING: Only " << spawned << " of " << m_requiredChests 
                  << " chests spawned! Not enough space." << std::endl;
    }
}
void DungeonLevel::SpawnEnemies(int level) {
    int enemyCount = level * 2 + 1;
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
    
    if (!IsLevelComplete()) {
        CheckChestsCollected();
    }
}

void DungeonLevel::CheckChestsCollected() {
    if (IsLevelComplete()) return;
    
    int collected = 0;
    for (Chest* chest : m_chests) {
        if (chest && chest->IsCollected()) {
            collected++;
        }
    }
    m_collectedChests = collected;
    
    if (IsLevelComplete()) {
        m_portal.Activate();
        std::cout << "[DungeonLevel] 🎉 ALL " << m_requiredChests << " CHESTS COLLECTED! PORTAL OPENED!" << std::endl;
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
    
    for (Chest* chest : m_chests) {
        GameWorld::GetInstance().DestroyGameObject(chest);
    }
    m_chests.clear();
    
    m_collectedChests = 0;
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
