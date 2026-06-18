#include "MazeGenerator.h"
#include <algorithm>
#include <iostream>

namespace rogalique {

MazeGenerator::MazeGenerator(int width, int height) 
    : m_width(width), m_height(height) {
    if (m_width % 2 == 0) m_width++;
    if (m_height % 2 == 0) m_height++;
    m_grid.resize(m_height, std::vector<Cell>(m_width));
}

void MazeGenerator::Generate(unsigned int seed) {
    if (seed == 0) {
        seed = static_cast<unsigned int>(std::random_device{}());
    }
    m_rng.seed(seed);
    std::cout << "[MazeGenerator] Generating maze with seed: " << seed << std::endl;
    
    for (int y = 0; y < m_height; y++) {
        for (int x = 0; x < m_width; x++) {
            m_grid[y][x] = Cell();
        }
    }
    
    std::stack<std::pair<int, int>> stack;
    int startX = 1 + (m_rng() % ((m_width - 2) / 2)) * 2;
    int startY = 1 + (m_rng() % ((m_height - 2) / 2)) * 2;
    
    stack.push({startX, startY});
    m_grid[startY][startX].visited = true;
    
    while (!stack.empty()) {
        auto [x, y] = stack.top();
        stack.pop();
        
        auto neighbors = GetUnvisitedNeighbors(x, y);
        if (!neighbors.empty()) {
            stack.push({x, y});
            
            Direction dir = neighbors[m_rng() % neighbors.size()];
            auto [nx, ny] = GetRandomNeighbor(x, y, dir);
            
            RemoveWall(x, y, nx, ny);
            m_grid[ny][nx].visited = true;
            stack.push({nx, ny});
        }
    }
    
    std::cout << "[MazeGenerator] Maze generation complete!" << std::endl;
}

void MazeGenerator::CarveRoom(int x, int y) {
    // Комната 6x6 (радиус 3)
    int roomSize = 3;
    for (int dy = -roomSize; dy <= roomSize; dy++) {
        for (int dx = -roomSize; dx <= roomSize; dx++) {
            int cx = x + dx, cy = y + dy;
            if (cx > 0 && cx < m_width - 1 && cy > 0 && cy < m_height - 1) {
                m_grid[cy][cx].isRoom = true;
                m_grid[cy][cx].visited = true;
                m_grid[cy][cx].walls[NORTH] = false;
                m_grid[cy][cx].walls[EAST] = false;
                m_grid[cy][cx].walls[SOUTH] = false;
                m_grid[cy][cx].walls[WEST] = false;
            }
        }
    }
    m_roomCenters.push_back({x, y});
}

void MazeGenerator::CarveRooms(int roomCount) {
    std::vector<sf::Vector2i> possibleRooms;
    
    // Ищем места для комнат
    for (int y = 3; y < m_height - 3; y += 4) {
        for (int x = 3; x < m_width - 3; x += 4) {
            if (m_grid[y][x].visited) {
                possibleRooms.push_back({x, y});
            }
        }
    }
    
    std::shuffle(possibleRooms.begin(), possibleRooms.end(), m_rng);
    int roomsToCarve = std::min(roomCount, (int)possibleRooms.size());
    
    // Создаём комнаты
    for (int i = 0; i < roomsToCarve; i++) {
        auto [x, y] = possibleRooms[i];
        CarveRoom(x, y);
        std::cout << "[MazeGenerator] Created room at (" << x << ", " << y << ")" << std::endl;
    }
    
    // ====== НОВЫЙ ПРОСТОЙ СПОСОБ ======
    // Просто убираем ВСЕ стены вокруг каждой комнаты
    std::cout << "[MazeGenerator] Unlocking " << m_roomCenters.size() << " rooms..." << std::endl;
    
    for (auto [cx, cy] : m_roomCenters) {
        // Проходим по всем клеткам вокруг комнаты (радиус 4)
        for (int dy = -4; dy <= 4; dy++) {
            for (int dx = -4; dx <= 4; dx++) {
                int nx = cx + dx;
                int ny = cy + dy;
                
                // Проверяем что в пределах карты
                if (nx > 0 && nx < m_width - 1 && ny > 0 && ny < m_height - 1) {
                    // Убираем все стены вокруг комнаты
                    m_grid[ny][nx].walls[NORTH] = false;
                    m_grid[ny][nx].walls[EAST] = false;
                    m_grid[ny][nx].walls[SOUTH] = false;
                    m_grid[ny][nx].walls[WEST] = false;
                    m_grid[ny][nx].visited = true;
                    
                    // Если это не комната - помечаем как проход
                    if (!m_grid[ny][nx].isRoom) {
                        m_grid[ny][nx].isRoom = false;
                    }
                }
            }
        }
        std::cout << "[MazeGenerator] Unlocked room at (" << cx << ", " << cy << ")" << std::endl;
    }
    
    std::cout << "[MazeGenerator] Carved and unlocked " << m_roomCenters.size() << " rooms" << std::endl;
}
void MazeGenerator::RemoveDeadEnds(float probability) {
    int removed = 0;
    for (int y = 1; y < m_height - 1; y++) {
        for (int x = 1; x < m_width - 1; x++) {
            if (!m_grid[y][x].visited || m_grid[y][x].isRoom) continue;
            
            int wallCount = 0;
            for (int i = 0; i < 4; i++) {
                if (m_grid[y][x].walls[i]) wallCount++;
            }
            
            if (wallCount == 3 && (m_rng() % 100) < (probability * 100)) {
                for (int i = 0; i < 4; i++) {
                    if (!m_grid[y][x].walls[i]) {
                        int opposite = (i + 2) % 4;
                        m_grid[y][x].walls[opposite] = false;
                        removed++;
                        break;
                    }
                }
            }
        }
    }
    std::cout << "[MazeGenerator] Removed " << removed << " dead ends" << std::endl;
}

bool MazeGenerator::IsWall(int x, int y) const {
    if (x < 0 || x >= m_width || y < 0 || y >= m_height) return true;
    if (!m_grid[y][x].visited) return true;
    if (m_grid[y][x].isRoom) return false;
    
    // Стена если есть стена сверху ИЛИ слева
    if (x % 2 == 0 && y % 2 == 0) return true;
    if (x % 2 == 1 && y % 2 == 0) {
        return m_grid[y][x].walls[NORTH] || m_grid[y][x].walls[SOUTH];
    }
    if (x % 2 == 0 && y % 2 == 1) {
        return m_grid[y][x].walls[EAST] || m_grid[y][x].walls[WEST];
    }
    return false;
}

bool MazeGenerator::IsWalkable(int x, int y) const {
    if (x < 0 || x >= m_width || y < 0 || y >= m_height) return false;
    // Временно: все посещённые клетки проходимы
    return m_grid[y][x].visited;
}
sf::Vector2i MazeGenerator::GetRoomCenter(int index) const {
    if (index < 0 || index >= (int)m_roomCenters.size()) {
        return {m_width / 2, m_height / 2};
    }
    return m_roomCenters[index];
}

bool MazeGenerator::IsValidCell(int x, int y) const {
    return x > 0 && x < m_width - 1 && y > 0 && y < m_height - 1;
}

bool MazeGenerator::HasUnvisitedNeighbors(int x, int y) const {
    return !GetUnvisitedNeighbors(x, y).empty();
}

std::vector<Direction> MazeGenerator::GetUnvisitedNeighbors(int x, int y) const {
    std::vector<Direction> neighbors;
    if (y > 1 && !m_grid[y-2][x].visited) neighbors.push_back(NORTH);
    if (x < m_width - 2 && !m_grid[y][x+2].visited) neighbors.push_back(EAST);
    if (y < m_height - 2 && !m_grid[y+2][x].visited) neighbors.push_back(SOUTH);
    if (x > 1 && !m_grid[y][x-2].visited) neighbors.push_back(WEST);
    return neighbors;
}

sf::Vector2i MazeGenerator::GetRandomNeighbor(int x, int y, Direction dir) const {
    switch(dir) {
        case NORTH: return {x, y-2};
        case EAST:  return {x+2, y};
        case SOUTH: return {x, y+2};
        case WEST:  return {x-2, y};
        default: return {x, y};
    }
}

void MazeGenerator::RemoveWall(int x1, int y1, int x2, int y2) {
    int mx = (x1 + x2) / 2;
    int my = (y1 + y2) / 2;
    
    if (x1 == x2) {
        m_grid[my][mx].walls[EAST] = false;
        m_grid[my][mx].walls[WEST] = false;
    } else {
        m_grid[my][mx].walls[NORTH] = false;
        m_grid[my][mx].walls[SOUTH] = false;
    }
    m_grid[my][mx].visited = true;
}

}
