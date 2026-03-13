#pragma once
#include <SFML/Audio.hpp>
#include <string>
#include <map>
#include <memory>

namespace SnakeGame
{
    class SoundManager
    {
    private:
        std::map<std::string, std::unique_ptr<sf::SoundBuffer>> m_buffers;
        std::map<std::string, std::unique_ptr<sf::Sound>> m_sounds;
        std::unique_ptr<sf::Music> m_music;

        bool m_soundsEnabled = true;
        bool m_musicEnabled = true;
        float m_soundVolume = 100.0f;
        float m_musicVolume = 70.0f;

    public:
        SoundManager();
        ~SoundManager() = default;

        
        bool LoadSound(const std::string& name, const std::string& filename);
        bool LoadMusic(const std::string& filename);

        void PlaySound(const std::string& name);
        void PlayMusic(bool loop = true);
        void StopMusic();
        void PauseMusic();

        void SetSoundsEnabled(bool enabled);
        void SetMusicEnabled(bool enabled);
        void SetSoundVolume(float volume);
        void SetMusicVolume(float volume);

        bool AreSoundsEnabled() const { return m_soundsEnabled; }
        bool IsMusicEnabled() const { return m_musicEnabled; }
        float GetSoundVolume() const { return m_soundVolume; }
        float GetMusicVolume() const { return m_musicVolume; }
    };
}