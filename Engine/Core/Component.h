#pragma once

namespace engine {

class GameObject;

class Component {
public:
    GameObject* gameObject = nullptr;
    
    virtual ~Component() = default;
    virtual void Update(float deltaTime) {}
    virtual void Render() {}
};

} // namespace engine
