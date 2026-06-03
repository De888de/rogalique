#include "GameWorld.h"
#include "Chest.h"
#include "CollisionComponent.h"
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
            Chest* chest = CreateGameObject<Chest>();
            chest->SetPosition(sf::Vector2f(distX(gen), distY(gen)));
        }
    }
    
    void GameWorld::CheckCollisions()
    {
        std::vector<CollisionComponent*> colliders;
        for (size_t i = 0; i < m_gameObjects.size(); ++i)
        {
            CollisionComponent* col = m_gameObjects[i]->GetComponent<CollisionComponent>();
            if (col)
                colliders.push_back(col);
        }
        
        for (size_t i = 0; i < colliders.size(); ++i)
        {
            for (size_t j = i + 1; j < colliders.size(); ++j)
            {
                if (colliders[i]->CheckCollision(colliders[j]))
                {
                    colliders[i]->OnCollisionEnter(colliders[j]);
                    colliders[j]->OnCollisionEnter(colliders[i]);
                }
            }
        }
    }
}
