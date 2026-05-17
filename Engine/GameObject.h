#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include "Vector2.h"
#include <string>

class GameObject {
public:
    GameObject();
    GameObject(const std::string& name);
    virtual ~GameObject();

    // Virtual methods for child classes
    virtual void Start();
    virtual void Update(float deltaTime);
    virtual void Render();
    virtual void Destroy();

    // Getters
    Vector2 GetPosition() const;
    Vector2 GetScale() const;
    bool IsActive() const;
    std::string GetName() const;

    // Setters
    void SetPosition(const Vector2& pos);
    void SetScale(const Vector2& scale);
    void SetActive(bool active);
    void SetName(const std::string& name);

protected:
    std::string m_name;
    Vector2 m_position;
    Vector2 m_scale;
    bool m_active;
};

#endif // GAMEOBJECT_H
