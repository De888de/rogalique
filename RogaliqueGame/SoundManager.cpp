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
        std::cout << "[SoundManager] Loaded sound: " << name << " from " << filename << std::endl;
    }

    void SoundManager::PlaySound(const std::string& name)
    {
        auto it = m_sounds.find(name);
        if (it != m_sounds.end())
        {
            it->second.play();
        }
        else
        {
            std::cout << "[SoundManager] Sound not found: " << name << std::endl;
        }
    }

    void SoundManager::PlayMusic(const std::string& filename)
    {
        if (!m_music.openFromFile(filename))
        {
            std::cout << "[SoundManager] Failed to load music: " << filename << std::endl;
            return;
        }
        m_music.setLoop(true);
        m_music.play();
        std::cout << "[SoundManager] Playing music: " << filename << std::endl;
    }

    void SoundManager::StopMusic()
    {
        m_music.stop();
    }

    void SoundManager::SetMusicVolume(float volume)
    {
        m_music.setVolume(volume);
    }

    void SoundManager::SetSoundVolume(float volume)
    {
        for (auto& pair : m_sounds)
        {
            pair.second.setVolume(volume);
        }
    }
}
