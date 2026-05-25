#include "ResourceSystem.h"
#include <iostream>

namespace rogalique
{
    ResourceSystem* ResourceSystem::Instance()
    {
        static ResourceSystem instance;
        return &instance;
    }
    
    ResourceSystem::~ResourceSystem()
    {
        Clear();
    }
    
    // ========== ТЕКСТУРЫ ==========
    void ResourceSystem::LoadTexture(const std::string& name, const std::string& sourcePath, bool isSmooth)
    {
        if (m_textures.find(name) != m_textures.end())
        {
            std::cout << "[ResourceSystem] Texture already loaded: " << name << std::endl;
            return;
        }
        
        sf::Texture* texture = new sf::Texture();
        if (!texture->loadFromFile(sourcePath))
        {
            std::cout << "[ResourceSystem] Failed to load texture: " << sourcePath << std::endl;
            delete texture;
            return;
        }
        
        texture->setSmooth(isSmooth);
        m_textures[name] = texture;
        std::cout << "[ResourceSystem] Loaded texture: " << name << " from " << sourcePath << std::endl;
    }
    
    const sf::Texture* ResourceSystem::GetTextureShared(const std::string& name) const
    {
        auto it = m_textures.find(name);
        if (it != m_textures.end())
            return it->second;
        
        std::cout << "[ResourceSystem] Texture not found: " << name << std::endl;
        return nullptr;
    }
    
    sf::Texture* ResourceSystem::GetTextureCopy(const std::string& name) const
    {
        auto it = m_textures.find(name);
        if (it != m_textures.end())
        {
            sf::Texture* copy = new sf::Texture();
            copy->loadFromImage(it->second->copyToImage());
            return copy;
        }
        return nullptr;
    }
    
    void ResourceSystem::DeleteSharedTexture(const std::string& name)
    {
        auto it = m_textures.find(name);
        if (it != m_textures.end())
        {
            delete it->second;
            m_textures.erase(it);
            std::cout << "[ResourceSystem] Deleted texture: " << name << std::endl;
        }
    }
    
    // ========== ТЕКСТУРНЫЕ КАРТЫ (АТЛАСЫ) ==========
    void ResourceSystem::LoadTextureMap(const std::string& name, const std::string& sourcePath,
                                        sf::Vector2u elementPixelSize, int totalElements, bool isSmooth)
    {
        sf::Texture* atlas = new sf::Texture();
        if (!atlas->loadFromFile(sourcePath))
        {
            std::cout << "[ResourceSystem] Failed to load texture map: " << sourcePath << std::endl;
            delete atlas;
            return;
        }
        
        atlas->setSmooth(isSmooth);
        
        int atlasWidth = atlas->getSize().x;
        int atlasHeight = atlas->getSize().y;
        int elementsPerRow = atlasWidth / elementPixelSize.x;
        
        std::vector<sf::Texture*> elements;
        
        for (int i = 0; i < totalElements && i < elementsPerRow * (atlasHeight / elementPixelSize.y); ++i)
        {
            sf::Texture* element = new sf::Texture();
            int x = (i % elementsPerRow) * elementPixelSize.x;
            int y = (i / elementsPerRow) * elementPixelSize.y;
            
            if (!element->loadFromImage(atlas->copyToImage(), sf::IntRect(x, y, elementPixelSize.x, elementPixelSize.y)))
            {
                std::cout << "[ResourceSystem] Failed to extract element " << i << " from texture map" << std::endl;
                delete element;
                continue;
            }
            
            element->setSmooth(isSmooth);
            elements.push_back(element);
        }
        
        m_textureMaps[name] = elements;
        delete atlas;
        std::cout << "[ResourceSystem] Loaded texture map: " << name << " with " << elements.size() << " elements" << std::endl;
    }
    
    const sf::Texture* ResourceSystem::GetTextureMapElementShared(const std::string& name, int elementIndex) const
    {
        auto it = m_textureMaps.find(name);
        if (it != m_textureMaps.end() && elementIndex >= 0 && elementIndex < static_cast<int>(it->second.size()))
            return it->second[elementIndex];
        
        std::cout << "[ResourceSystem] Texture map element not found: " << name << "[" << elementIndex << "]" << std::endl;
        return nullptr;
    }
    
    sf::Texture* ResourceSystem::GetTextureMapElementCopy(const std::string& name, int elementIndex) const
    {
        auto it = m_textureMaps.find(name);
        if (it != m_textureMaps.end() && elementIndex >= 0 && elementIndex < static_cast<int>(it->second.size()))
        {
            sf::Texture* copy = new sf::Texture();
            copy->loadFromImage(it->second[elementIndex]->copyToImage());
            return copy;
        }
        return nullptr;
    }
    
    int ResourceSystem::GetTextureMapElementsCount(const std::string& name) const
    {
        auto it = m_textureMaps.find(name);
        if (it != m_textureMaps.end())
            return static_cast<int>(it->second.size());
        return 0;
    }
    
    void ResourceSystem::DeleteSharedTextureMap(const std::string& name)
    {
        auto it = m_textureMaps.find(name);
        if (it != m_textureMaps.end())
        {
            for (auto* tex : it->second)
                delete tex;
            m_textureMaps.erase(it);
            std::cout << "[ResourceSystem] Deleted texture map: " << name << std::endl;
        }
    }
    
    // ========== ШРИФТЫ ==========
    void ResourceSystem::LoadFont(const std::string& name, const std::string& sourcePath)
    {
        if (m_fonts.find(name) != m_fonts.end())
        {
            std::cout << "[ResourceSystem] Font already loaded: " << name << std::endl;
            return;
        }
        
        sf::Font* font = new sf::Font();
        if (!font->loadFromFile(sourcePath))
        {
            std::cout << "[ResourceSystem] Failed to load font: " << sourcePath << std::endl;
            delete font;
            return;
        }
        
        m_fonts[name] = font;
        std::cout << "[ResourceSystem] Loaded font: " << name << std::endl;
    }
    
    const sf::Font* ResourceSystem::GetFont(const std::string& name) const
    {
        auto it = m_fonts.find(name);
        if (it != m_fonts.end())
            return it->second;
        
        std::cout << "[ResourceSystem] Font not found: " << name << std::endl;
        return nullptr;
    }
    
    // ========== ЗВУКИ ==========
    void ResourceSystem::LoadSoundBuffer(const std::string& name, const std::string& sourcePath)
    {
        if (m_soundBuffers.find(name) != m_soundBuffers.end())
        {
            std::cout << "[ResourceSystem] Sound already loaded: " << name << std::endl;
            return;
        }
        
        sf::SoundBuffer* buffer = new sf::SoundBuffer();
        if (!buffer->loadFromFile(sourcePath))
        {
            std::cout << "[ResourceSystem] Failed to load sound: " << sourcePath << std::endl;
            delete buffer;
            return;
        }
        
        m_soundBuffers[name] = buffer;
        std::cout << "[ResourceSystem] Loaded sound: " << name << std::endl;
    }
    
    const sf::SoundBuffer* ResourceSystem::GetSoundBuffer(const std::string& name) const
    {
        auto it = m_soundBuffers.find(name);
        if (it != m_soundBuffers.end())
            return it->second;
        
        std::cout << "[ResourceSystem] Sound not found: " << name << std::endl;
        return nullptr;
    }
    
    // ========== ОЧИСТКА ==========
    void ResourceSystem::Clear()
    {
        DeleteAllTextures();
        DeleteAllTextureMaps();
        
        for (auto& pair : m_fonts)
            delete pair.second;
        m_fonts.clear();
        
        for (auto& pair : m_soundBuffers)
            delete pair.second;
        m_soundBuffers.clear();
        
        std::cout << "[ResourceSystem] All resources cleared" << std::endl;
    }
    
    void ResourceSystem::DeleteAllTextures()
    {
        for (auto& pair : m_textures)
            delete pair.second;
        m_textures.clear();
    }
    
    void ResourceSystem::DeleteAllTextureMaps()
    {
        for (auto& pair : m_textureMaps)
            for (auto* tex : pair.second)
                delete tex;
        m_textureMaps.clear();
    }
}
