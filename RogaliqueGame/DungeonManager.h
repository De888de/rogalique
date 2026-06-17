#pragma once
#include "DungeonLevel.h"
#include "Player.h"
#include <memory>

namespace rogalique {

class DungeonManager {
public:
    DungeonManager(Player* player);
    ~DungeonManager();
    
    void GenerateNextLevel();
    void Update(float deltaTime);
    void Clear();
    
    DungeonLevel* GetCurrentLevel() { return m_currentLevel.get(); }
    int GetLevelNumber() const { return m_levelNumber; }
    bool IsTransitioning() const { return m_isTransitioning; }
    float GetTransitionProgress() const { return m_transitionTimer / m_transitionDuration; }
    
private:
    void StartTransition();
    void CompleteTransition();
    
    Player* m_player = nullptr;
    std::unique_ptr<DungeonLevel> m_currentLevel;
    int m_levelNumber = 0;
    
    bool m_isTransitioning = false;
    float m_transitionTimer = 0.0f;
    float m_transitionDuration = 1.5f;
};

}
