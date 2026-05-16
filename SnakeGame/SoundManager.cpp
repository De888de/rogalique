#include "SoundManager.h"
#include <iostream>


namespace SnakeGame
{
    SoundManager::SoundManager()
    {
        m_music = std::make_unique<sf::Music>();
    }

    bool SoundManager::LoadSound(const std::string& name, const std::string& filename)
    {
        auto buffer = std::make_unique<sf::SoundBuffer>();
        if (!buffer->loadFromFile(filename))
            return false;

        auto sound = std::make_unique<sf::Sound>();
        sound->setBuffer(*buffer);
        sound->setVolume(m_soundVolume);

        m_buffers[name] = std::move(buffer);
        m_sounds[name] = std::move(sound);
        return true;
    }

    bool SoundManager::LoadMusic(const std::string& filename)
    {
        return m_music->openFromFile(filename);
    }

    void SoundManager::PlaySound(const std::string& name)
    {
        if (!m_soundsEnabled) return;

        auto it = m_sounds.find(name);
        if (it != m_sounds.end())
        {
            it->second->play();
        }
    }

    void SoundManager::PlayMusic(bool loop)
    {
        if (!m_musicEnabled || !m_music) return;

        m_music->setLoop(loop);
        m_music->setVolume(m_musicVolume);
        m_music->play();
    }

    void SoundManager::StopMusic()
    {
        if (m_music)
            m_music->stop();
    }

    void SoundManager::PauseMusic()
    {
        if (m_music)
            m_music->pause();
    }

    void SoundManager::SetSoundsEnabled(bool enabled)
    {
        m_soundsEnabled = enabled;
        if (!enabled)
        {
            for (auto& pair : m_sounds)
                pair.second->stop();
        }
    }

    void SoundManager::SetMusicEnabled(bool enabled)
    {
        m_musicEnabled = enabled;
        if (m_music)
        {
            if (enabled)
                m_music->play();
            else
                m_music->pause();
        }
    }

    void SoundManager::SetSoundVolume(float volume)
    {
        m_soundVolume = volume;
        for (auto& pair : m_sounds)
            pair.second->setVolume(volume);
    }

    void SoundManager::SetMusicVolume(float volume)
    {
        m_musicVolume = volume;
        if (m_music)
            m_music->setVolume(volume);
    }
}