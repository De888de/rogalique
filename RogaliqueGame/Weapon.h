#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>

namespace rogalique {

    class Bullet;
    class Player;
    
    class Weapon {
    public:
        Weapon(std::string name, int maxAmmo, float fireRate, int damage, float bulletSpeed = 600.0f, float muzzleOffset = 32.0f);
        
        void Update(float deltaTime);
        void Render(sf::RenderWindow& window, sf::Vector2f playerPos, sf::Vector2f direction);
        
        Bullet* Shoot(sf::Vector2f playerPos, sf::Vector2f targetPos);
        void Reload();
        
        bool CanShoot() const { return m_shootCooldown <= 0.0f && m_currentAmmo > 0; }
        
        int GetCurrentAmmo() const { return m_currentAmmo; }
        int GetMaxAmmo() const { return m_maxAmmo; }
        int GetDamage() const { return m_damage; }
        std::string GetName() const { return m_name; }
        
        void SetMuzzleOffset(float offset) { m_muzzleOffset = offset; }
        
    private:
        sf::Vector2f CalculateMuzzlePosition(sf::Vector2f playerPos, sf::Vector2f direction) const;
        void UpdateUI() const;
        
        std::string m_name;
        int m_maxAmmo;
        int m_currentAmmo;
        float m_fireRate;
        float m_shootCooldown;
        int m_damage;
        float m_bulletSpeed;
        float m_muzzleOffset;
        
        // Визуал оружия
        sf::RectangleShape m_weaponSprite;
    };
}
