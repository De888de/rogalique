#pragma once
#include <vector>
#include <memory>
#include <SFML/Graphics.hpp>
#include "Component.h"

namespace rogalique
{
    class GameObject
    {
    public:
        GameObject();
        virtual ~GameObject();
        
        void Update(float deltaTime);
        void Render(sf::RenderWindow& window);
        
        template<typename T, typename... Args>
        T* AddComponent(Args&&... args)
        {
            T* comp = new T(this, std::forward<Args>(args)...);
            m_components.push_back(std::unique_ptr<T>(comp));
            return comp;
        }
        
        template<typename T>
        T* GetComponent() const
        {
            for (const auto& comp : m_components)
            {
                T* casted = dynamic_cast<T*>(comp.get());
                if (casted) return casted;
            }
            return nullptr;
        }
        
        void SetPosition(const sf::Vector2f& pos) { m_position = pos; }
        sf::Vector2f GetPosition() const { return m_position; }
        
    protected:
        sf::Vector2f m_position;
        
    private:
        std::vector<std::unique_ptr<Component>> m_components;
    };
}
