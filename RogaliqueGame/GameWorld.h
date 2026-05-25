#pragma once
#include <vector>
#include <SFML/Graphics.hpp>
#include "RogaliqueGameObject.h"

namespace rogalique
{
    class GameWorld
    {
    public:
        static GameWorld& GetInstance();
        
        void Update(float deltaTime);
        void Render(sf::RenderWindow& window);
        void LateUpdate();
        
        template<typename T, typename... Args>
        T* CreateGameObject(Args&&... args)
        {
            T* obj = new T(std::forward<Args>(args)...);
            m_gameObjects.push_back(obj);
            return obj;
        }
        
        template<typename T>
        T* CreateGameObject()
        {
            T* obj = new T();
            m_gameObjects.push_back(obj);
            return obj;
        }
        
        void DestroyGameObject(RogaliqueGameObject* obj);
        void DestroyGameObjectImmediate(RogaliqueGameObject* obj);
        void Clear();
        
        void SpawnChests(int count, float worldWidth, float worldHeight);
        const std::vector<RogaliqueGameObject*>& GetAllGameObjects() const { return m_gameObjects; }
        
    private:
        GameWorld() = default;
        ~GameWorld() = default;
        
        std::vector<RogaliqueGameObject*> m_gameObjects;
        std::vector<RogaliqueGameObject*> m_markedForDestroy;
    };
}
