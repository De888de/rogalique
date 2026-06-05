#pragma once
#include "Component.h"
#include <vector>
#include <memory>
#include <typeindex>
#include <unordered_map>

namespace engine {

class GameObject {
public:
    virtual ~GameObject() = default;
    
    template<typename T, typename... Args>
    T* AddComponent(Args&&... args) {
        static_assert(std::is_base_of_v<Component, T>, "T must be a Component");
        auto* comp = new T(std::forward<Args>(args)...);
        comp->gameObject = this;
        m_components[std::type_index(typeid(T))] = std::unique_ptr<Component>(comp);
        return comp;
    }
    
    template<typename T>
    T* GetComponent() {
        auto it = m_components.find(std::type_index(typeid(T)));
        if (it != m_components.end()) {
            return static_cast<T*>(it->second.get());
        }
        return nullptr;
    }
    
    void Update(float deltaTime) {
        for (auto& [type, comp] : m_components) {
            comp->Update(deltaTime);
        }
    }
    
    void Render() {
        for (auto& [type, comp] : m_components) {
            comp->Render();
        }
    }
    
private:
    std::unordered_map<std::type_index, std::unique_ptr<Component>> m_components;
};

} // namespace engine
