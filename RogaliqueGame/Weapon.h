#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>

namespace rogalique {

    class Weapon {
    public:
        Weapon() 
            : m_name("Iron Pistol")
            , m_maxAmmo(20)
            , m_currentAmmo(20)
            , m_fireRate(0.25f)
            , m_damage(10)
            , m_shootCooldown(0.0f)
        {
            std::cout << "[Weapon] Created: " << m_name << " | Ammo: " << m_currentAmmo << "/" << m_maxAmmo << std::endl;
        }
        
        Weapon(std::string name, int maxAmmo, float fireRate, int damage)
            : m_name(name)
            , m_maxAmmo(maxAmmo)
            , m_currentAmmo(maxAmmo)
            , m_fireRate(fireRate)
            , m_damage(damage)
            , m_shootCooldown(0.0f)
        {
            std::cout << "[Weapon] Created: " << m_name << " | Ammo: " << m_currentAmmo << "/" << m_maxAmmo << " | Damage: " << m_damage << std::endl;
        }
        
        void Update(float deltaTime) {
            if (m_shootCooldown > 0) {
                m_shootCooldown -= deltaTime;
            }
        }
        
        bool CanShoot() const {
            return m_shootCooldown <= 0.0f && m_currentAmmo > 0;
        }
        
        bool Shoot() {
            if (!CanShoot()) {
                if (m_currentAmmo <= 0) {
                    std::cout << "[Weapon] " << m_name << " is EMPTY! Reload with R!" << std::endl;
                }
                return false;
            }
            
            m_currentAmmo--;
            m_shootCooldown = m_fireRate;
            std::cout << "[Weapon] " << m_name << " BANG! Ammo: " << m_currentAmmo << "/" << m_maxAmmo << std::endl;
            return true;
        }
        
        void Reload() {
            m_currentAmmo = m_maxAmmo;
            std::cout << "[Weapon] " << m_name << " reloaded! Ammo: " << m_currentAmmo << "/" << m_maxAmmo << std::endl;
        }
        
        int GetCurrentAmmo() const { return m_currentAmmo; }
        int GetMaxAmmo() const { return m_maxAmmo; }
        int GetDamage() const { return m_damage; }
        std::string GetName() const { return m_name; }
        float GetFireRate() const { return m_fireRate; }
        
    private:
        std::string m_name;
        int m_maxAmmo;
        int m_currentAmmo;
        float m_fireRate;
        float m_shootCooldown;
        int m_damage;
    };
    
}
