#include "Weapon.h"
#include "Bullet.h"
#include "GameWorld.h"
#include "Application.h"
#include "SoundManager.h"
#include <cmath>
#include <iostream>

namespace rogalique {

    Weapon::Weapon(std::string name, int maxAmmo, float fireRate, int damage, float bulletSpeed, float muzzleOffset)
        : m_name(name)
        , m_maxAmmo(maxAmmo)
        , m_currentAmmo(maxAmmo)
        , m_fireRate(fireRate)
        , m_shootCooldown(0.0f)
        , m_damage(damage)
        , m_bulletSpeed(bulletSpeed)
        , m_muzzleOffset(muzzleOffset)
    {
        // Настройка визуала оружия
        m_weaponSprite.setSize(sf::Vector2f(35, 12));
        m_weaponSprite.setFillColor(sf::Color(200, 200, 50));
        m_weaponSprite.setOrigin(5, 6);
        
        std::cout << "[Weapon] Created: " << m_name << " | Ammo: " << m_currentAmmo << "/" << m_maxAmmo 
                  << " | Damage: " << m_damage << " | Speed: " << m_bulletSpeed << std::endl;
    }
    
    void Weapon::Update(float deltaTime)
    {
        if (m_shootCooldown > 0.0f) {
            m_shootCooldown -= deltaTime;
            if (m_shootCooldown < 0.0f) {
                m_shootCooldown = 0.0f;
            }
        }
    }
    
    void Weapon::Render(sf::RenderWindow& window, sf::Vector2f playerPos, sf::Vector2f direction)
    {
        // Вычисляем угол поворота оружия
        float angle = std::atan2(direction.y, direction.x) * 180.0f / 3.14159265f;
        
        m_weaponSprite.setRotation(angle);
        m_weaponSprite.setPosition(playerPos.x + 18.f, playerPos.y - 5.f);
        
        window.draw(m_weaponSprite);
    }
    
    sf::Vector2f Weapon::CalculateMuzzlePosition(sf::Vector2f playerPos, sf::Vector2f direction) const
    {
        return playerPos + direction * m_muzzleOffset;
    }
    
    void Weapon::UpdateUI() const
    {
        if (g_Application) {
            g_Application->UpdateUI();
        }
    }
    
    Bullet* Weapon::Shoot(sf::Vector2f playerPos, sf::Vector2f targetPos)
    {
        if (!CanShoot()) {
            if (m_currentAmmo <= 0) {
                std::cout << "[Weapon] " << m_name << " is EMPTY! Reload with R!" << std::endl;
            }
            return nullptr;
        }
        
        // Вычисляем направление на цель
        sf::Vector2f dir = targetPos - playerPos;
        float len = std::sqrt(dir.x * dir.x + dir.y * dir.y);
        if (len > 0.001f) dir /= len;
        
        // Вычисляем позицию дула
        sf::Vector2f muzzlePos = CalculateMuzzlePosition(playerPos, dir);
        
        // Тратим патрон и ставим кулдаун
        m_currentAmmo--;
        m_shootCooldown = m_fireRate;
        
        std::cout << "[Weapon] " << m_name << " BANG! Ammo: " << m_currentAmmo << "/" << m_maxAmmo << std::endl;
        
        // Обновляем UI
        UpdateUI();
        
        // Создаём пулю
        return new Bullet(muzzlePos, dir, m_bulletSpeed, m_damage);
    }
    
    void Weapon::Reload()
    {
        m_currentAmmo = m_maxAmmo;
        std::cout << "[Weapon] " << m_name << " reloaded! Ammo: " << m_currentAmmo << "/" << m_maxAmmo << std::endl;
        UpdateUI();
    }
}
