#pragma once
#include <vector>
#include <stack>
#include <random>
#include <SFML/System/Vector2.hpp>

namespace rogalique {

struct Cell {
    bool visited = false;
    bool walls[4] = {true, true, true, true};
    bool isRoom = false;
};

enum Direction { NORTH = 0, EAST = 1, SOUTH = 2, WEST = 3 };

class MazeGenerator {
public:
    MazeGenerator(int width = 41, int height = 41);
    void Generate(unsigned int seed = 0);
    void CarveRooms(int roomCount);
    void RemoveDeadEnds(float probability = 0.3f);
    
    bool IsWall(int x, int y) const;
    bool IsWalkable(int x, int y) const;
    sf::Vector2i GetRoomCenter(int index) const;
    std::vector<sf::Vector2i> GetRooms() const { return m_roomCenters; }
    int GetWidth() const { return m_width; }
    int GetHeight() const { return m_height; }
    const std::vector<std::vector<Cell>>& GetGrid() const { return m_grid; }

private:
    bool IsValidCell(int x, int y) const;
    bool HasUnvisitedNeighbors(int x, int y) const;
    std::vector<Direction> GetUnvisitedNeighbors(int x, int y) const;
    void RemoveWall(int x1, int y1, int x2, int y2);
    void CarveRoom(int x, int y);
    sf::Vector2i GetRandomNeighbor(int x, int y, Direction dir) const;

    int m_width, m_height;
    std::vector<std::vector<Cell>> m_grid;
    std::vector<sf::Vector2i> m_roomCenters;
    std::mt19937 m_rng;
};

}
