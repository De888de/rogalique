#pragma once
#include "MazeGenerator.h"
#include "Portal.h"
#include "Enemy.h"
#include "Wall.h"
#include <vector>
#include <memory>

namespace rogalique {

class Player;

class DungeonLevel {
public:
    DungeonLevel();
    void Generate(int levelNumber, Player* player);
    void Update(float deltaTime);
    void Clear();
    
    MazeGenerator& GetMaze() { return m_maze; }
    Portal& GetPortal() { return m_portal; }
    std::vector<Enemy*>& GetEnemies() { return m_enemies; }
    std::vector<Wall*>& GetWalls() { return m_walls; }
    sf::Vector2f GetPlayerStart() const { return m_playerStart; }
    int GetLevelNumber() const { return m_levelNumber; }
    bool IsAllEnemiesDefeated() const { return m_allEnemiesDefeated; }

private:
    void SpawnWalls();
    void SpawnEnemies(int level);
    void CheckEnemiesDefeated();
    sf::Vector2f MazeToWorld(int mx, int my) const;
    sf::Vector2i WorldToMaze(float wx, float wy) const;

    int m_levelNumber = 0;
    MazeGenerator m_maze;
    std::vector<Enemy*> m_enemies;
    std::vector<Wall*> m_walls;
    Portal m_portal;
    sf::Vector2f m_playerStart;
    bool m_allEnemiesDefeated = false;
    static constexpr float CELL_SIZE = 32.0f;
};

}
