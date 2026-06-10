#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <ctime>
#include <sstream>

namespace rogalique {

    class Logger {
    public:
        static Logger& GetInstance() {
            static Logger instance;
            return instance;
        }
        
        void Log(const std::string& message) {
            std::string timestamp = GetTimestamp();
            std::string logMessage = "[" + timestamp + "] " + message;
            
            std::cout << logMessage << std::endl;
            
            std::ofstream file("game_log.txt", std::ios::app);
            if (file.is_open()) {
                file << logMessage << std::endl;
                file.close();
            }
        }
        
        void LogEvent(const std::string& event, const std::string& details = "") {
            std::string msg = "[EVENT] " + event;
            if (!details.empty()) {
                msg += " | " + details;
            }
            Log(msg);
        }
        
        void LogError(const std::string& error, const std::string& details = "") {
            std::string msg = "[ERROR] " + error;
            if (!details.empty()) {
                msg += " | " + details;
            }
            Log(msg);
        }
        
        void LogPlayerDamage(int health) {
            LogEvent("Player Damaged", "Health: " + std::to_string(health));
        }
        
        void LogPlayerDeath() {
            LogEvent("Player Died", "GAME OVER");
        }
        
        void LogEnemyDeath(const std::string& type) {
            LogEvent("Enemy Died", "Type: " + type);
        }
        
        void LogWeaponPickup(const std::string& weaponName) {
            LogEvent("Weapon Pickup", "Weapon: " + weaponName);
        }
        
        void ClearLog() {
            std::ofstream file("game_log.txt", std::ios::trunc);
            file.close();
            Log("=== GAME LOG STARTED ===");
        }
        
    private:
        Logger() {
            ClearLog();
        }
        
        std::string GetTimestamp() {
            time_t now = time(nullptr);
            struct tm timeinfo;
            char buf[80];
            
            // Используем безопасную версию localtime_s
            localtime_s(&timeinfo, &now);
            strftime(buf, sizeof(buf), "%H:%M:%S", &timeinfo);
            return std::string(buf);
        }
    };
    
    // Макросы для удобства
    #define LOG(message) Logger::GetInstance().Log(message)
    #define LOG_EVENT(event, details) Logger::GetInstance().LogEvent(event, details)
    #define LOG_ERROR(error, details) Logger::GetInstance().LogError(error, details)
    #define LOG_PLAYER_DAMAGE(health) Logger::GetInstance().LogPlayerDamage(health)
    #define LOG_PLAYER_DEATH() Logger::GetInstance().LogPlayerDeath()
    #define LOG_ENEMY_DEATH(type) Logger::GetInstance().LogEnemyDeath(type)
    #define LOG_WEAPON_PICKUP(name) Logger::GetInstance().LogWeaponPickup(name)
}
