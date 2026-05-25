#include "GameWorld.h"
#include "Chest.h"
#include <algorithm>
#include <iostream>
#include <random>

namespace rogalique
{
    GameWorld& GameWorld::GetInstance()
    {
        static GameWorld instance;
        return instance;
    }
    
    void GameWorld::Update(float deltaTime)
    {
        for (auto* obj : m_gameObjects)
        {
            if (obj)
                obj->Update(deltaTime);
        }
    }
    
    void GameWorld::Render(sf::RenderWindow& window)
    {
        for (auto* obj : m_gameObjects)
        {
            if (obj)
                obj->Render(window);
        }
    }
    
    void GameWorld::LateUpdate()
    {
        for (auto* obj : m_markedForDestroy)
        {
            auto it = std::find(m_gameObjects.begin(), m_gameObjects.end(), obj);
            if (it != m_gameObjects.end())
            {
                m_gameObjects.erase(it);
                delete obj;
            }
        }
        m_markedForDestroy.clear();
    }
    
    void GameWorld::DestroyGameObject(RogaliqueGameObject* obj)
    {
        if (std::find(m_markedForDestroy.begin(), m_markedForDestroy.end(), obj) == m_markedForDestroy.end())
        {
            m_markedForDestroy.push_back(obj);
        }
    }
    
    void GameWorld::DestroyGameObjectImmediate(RogaliqueGameObject* obj)
    {
        auto it = std::find(m_gameObjects.begin(), m_gameObjects.end(), obj);
        if (it != m_gameObjects.end())
        {
            m_gameObjects.erase(it);
            delete obj;
        }
    }
    
    void GameWorld::Clear()
    {
        for (auto* obj : m_gameObjects)
            delete obj;
        m_gameObjects.clear();
        m_markedForDestroy.clear();
    }
    
    void GameWorld::SpawnChests(int count, float worldWidth, float worldHeight)
    {
        std::random_device rd;
        std::mt19937 gen(rd());
        
        std::uniform_real_distribution<float> distX(50, worldWidth - 50);
        std::uniform_real_distribution<float> distY(50, worldHeight - 50);
        
        for (int i = 0; i < count; ++i)
        {
            auto* chest = CreateGameObject<Chest>();
            chest->SetPosition(sf::Vector2f(distX(gen), distY(gen)));
            std::cout << "[GameWorld] Spawned chest " << i+1 << " at (" 
                      << chest->GetPosition().x << ", " << chest->GetPosition().y << ")" << std::endl;
        }
    }
}
