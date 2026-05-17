#include "pch.h"
#include "GameObject.h"
#include <iostream>

GameObject::GameObject() 
    : m_name("GameObject"), m_position(0, 0), m_scale(1, 1), m_active(true)
{
}

GameObject::GameObject(const std::string& name) 
    : m_name(name), m_position(0, 0), m_scale(1, 1), m_active(true)
{
}

GameObject::~GameObject()
{
}

void GameObject::Start()
{
    std::cout << "GameObject '" << m_name << "' started" << std::endl;
}

void GameObject::Update(float deltaTime)
{
}

void GameObject::Render()
{
}

void GameObject::Destroy()
{
    m_active = false;
    std::cout << "GameObject '" << m_name << "' destroyed" << std::endl;
}

void GameObject::SetPosition(const Vector2& pos)
{
    m_position = pos;
}

Vector2 GameObject::GetPosition() const
{
    return m_position;
}

void GameObject::SetScale(const Vector2& scale)
{
    m_scale = scale;
}

Vector2 GameObject::GetScale() const
{
    return m_scale;
}

void GameObject::SetActive(bool active)
{
    m_active = active;
}

bool GameObject::IsActive() const
{
    return m_active;
}

std::string GameObject::GetName() const
{
    return m_name;
}

void GameObject::SetName(const std::string& name)
{
    m_name = name;
}
