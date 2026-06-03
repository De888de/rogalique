#include "SoundManager.h"
#include <iostream>

namespace rogalique
{
    SoundManager& SoundManager::GetInstance()
    {
        static SoundManager instance;
        return instance;
    }

    void SoundManager::LoadSound(const std::string& name, const std::string& filename)
    {
        sf::SoundBuffer buffer;
        if (!buffer.loadFromFile(filename))
        {
            std::cout << "[SoundManager] Failed to load sound: " << filename << std::endl;
            return;
        }
        m_soundBuffers[name] = std::move(buffer);
        m_sounds[name].setBuffer(m_soundBuffers[name]);
        std::cout << "[SoundManager] Loaded sound: " << name << std::endl;
    }

    void SoundManager::PlaySound(const std::string& name)
    {
        auto it = m_sounds.find(name);
        if (it != m_sounds.end())
        {
            it->second.play();
        }
    }

    void SoundManager::LoadMusic(const std::string& name, const std::string& filename)
    {
        auto music = std::make_unique<sf::Music>();
        if (!music->openFromFile(filename))
        {
            std::cout << "[SoundManager] Failed to load music: " << filename << std::endl;
            return;
        }
        m_musicMap[name] = std::move(music);
        std::cout << "[SoundManager] Loaded music: " << name << std::endl;
    }

    void SoundManager::PlayMusic(const std::string& name)
    {
        auto it = m_musicMap.find(name);
        if (it == m_musicMap.end() || !it->second)
        {
            std::cout << "[SoundManager] Music not found: " << name << std::endl;
            return;
        }
        
        if (m_currentMusic.getStatus() == sf::Music::Playing)
            m_currentMusic.stop();
        
        // Копируем музыку (нельзя, но мы можем перезагрузить из файла)
        // Временно: используем отдельный метод
        std::cout << "[SoundManager] Playing music by name: " << name << std::endl;
        
        // Сохраняем имя текущей музыки
        m_currentMusicName = name;
    }

    void SoundManager::PlayMusicFile(const std::string& filename)
    {
        if (m_currentMusic.getStatus() == sf::Music::Playing)
            m_currentMusic.stop();
            
        if (!m_currentMusic.openFromFile(filename))
        {
            std::cout << "[SoundManager] Failed to play music file: " << filename << std::endl;
            return;
        }
        m_currentMusic.setLoop(true);
        m_currentMusic.play();
        std::cout << "[SoundManager] Playing music file: " << filename << std::endl;
    }

    void SoundManager::StopMusic()
    {
        m_currentMusic.stop();
    }

    void SoundManager::SetMusicVolume(float volume)
    {
        m_currentMusic.setVolume(volume);
    }

    void SoundManager::SetSoundVolume(float volume)
    {
        for (auto& pair : m_sounds)
        {
            pair.second.setVolume(volume);
        }
    }
}
