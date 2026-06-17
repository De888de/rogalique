#include "DungeonManager.h"
#include "SoundManager.h"
#include "TransformComponent.h"
#include <iostream>

namespace rogalique {

DungeonManager::DungeonManager(Player* player) 
    : m_player(player) {
    std::cout << "[DungeonManager] Created" << std::endl;
}

DungeonManager::~DungeonManager() {
    Clear();
}

void DungeonManager::GenerateNextLevel() {
    m_levelNumber++;
    std::cout << "[DungeonManager] Generating level " << m_levelNumber << std::endl;
    
    if (m_currentLevel) {
        m_currentLevel->Clear();
    }
    
    m_currentLevel = std::make_unique<DungeonLevel>();
    m_currentLevel->Generate(m_levelNumber, m_player);
    
    if (m_player) {
        sf::Vector2f startPos = m_currentLevel->GetPlayerStart();
        auto* transform = m_player->GetComponent<TransformComponent>();
        if (transform) {
            transform->SetPosition(startPos);
        }
    }
    
    SoundManager::GetInstance().PlayMusicFile("RogaliqueGame/Resources/Sounds/dungeon.WAV");
    m_isTransitioning = false;
}

void DungeonManager::Update(float deltaTime) {
    if (!m_currentLevel || !m_player) return;
    
    if (m_isTransitioning) {
        m_transitionTimer += deltaTime;
        if (m_transitionTimer >= m_transitionDuration) {
            CompleteTransition();
        }
        return;
    }
    
    m_currentLevel->Update(deltaTime);
    
    Portal& portal = m_currentLevel->GetPortal();
    if (portal.IsActive()) {
        sf::Vector2f playerPos = m_player->GetPosition();
        if (portal.IsNear(playerPos, 35.0f)) {
            std::cout << "[DungeonManager] Player touched portal!" << std::endl;
            StartTransition();
        }
    }
}

void DungeonManager::StartTransition() {
    if (m_isTransitioning) return;
    m_isTransitioning = true;
    m_transitionTimer = 0.0f;
    SoundManager::GetInstance().PlaySound("click");
}

void DungeonManager::CompleteTransition() {
    m_isTransitioning = false;
    GenerateNextLevel();
}

void DungeonManager::Clear() {
    if (m_currentLevel) {
        m_currentLevel->Clear();
        m_currentLevel.reset();
    }
    m_levelNumber = 0;
    m_isTransitioning = false;
}

}
