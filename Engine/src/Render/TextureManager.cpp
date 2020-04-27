#include "precomp.h"
#include "TextureManager.h"

#include "Render/Texture.h"
#include "Render/Renderer.h"

namespace Engine
{

    bool TextureManager::CreateTexture(Renderer* renderer_, std::string name_, std::string path_)
    {
        if (m_Textures.find(name_) != m_Textures.end()) {
            LOG_ERROR("Attempting to create a texture twice! name: {}, path: {}", name_, path_);
            return false;
        }

        m_Textures.emplace(name_, std::make_unique<Texture>(renderer_, path_));

        return m_Textures.at(name_)->m_Texture != nullptr;
    }

    bool TextureManager::CreateTexture(Renderer* renderer_, std::string name_, std::string path_, std::string pathShadow_)
    {
        if (m_Textures.find(name_) != m_Textures.end()) {
            LOG_ERROR("Attempting to create a texture twice! name: {}, path: {}", name_, path_);
            return false;
        }

        m_Textures.emplace(name_, std::make_unique<Texture>(renderer_, path_, pathShadow_));

        return m_Textures.at(name_)->m_Texture != nullptr;
    }

    Texture* TextureManager::GetTexture(std::string name_)
    {
        if (m_Textures.find(name_) == m_Textures.end()) {

            LOG_ERROR("Attempting to get a texture that doesn't exist! name: {}", name_);
            return nullptr;
        }

        return m_Textures.at(name_).get();
    }
    bool TextureManager::CreateAnimationTexture(vec2 size_, int numOfParts_, Renderer* renderer_, std::string name_, std::string path_) {
        
            if (m_AnimationTextures.find(name_) != m_AnimationTextures.end()) {

                LOG_ERROR("Attempting to create a animation texture twice! name: {}, path: {}", name_, path_);
                return false;
            }

            m_AnimationTextures.emplace(name_, std::make_unique<AnimationTexture>(size_, numOfParts_, renderer_, path_));

            return m_AnimationTextures.at(name_)->m_TextureSheet != nullptr;
       
    }
    bool TextureManager::CreateAnimationTexture(vec2 size_, int numOfParts_, Renderer* renderer_, std::string name_, std::string path_, std::string pathShadow_) {
        
        if (m_AnimationTextures.find(name_) != m_AnimationTextures.end()) {

            LOG_ERROR("Attempting to create a animation texture twice! name: {}, path: {}", name_, path_);
            return false;
        }

        m_AnimationTextures.emplace(name_, std::make_unique<AnimationTexture>(size_, numOfParts_, renderer_, path_, pathShadow_));

        return m_AnimationTextures.at(name_)->m_TextureSheet != nullptr;
    }

    AnimationTexture* TextureManager::GetAnimationTexture(std::string name_) {
        
        if (m_AnimationTextures.find(name_) == m_AnimationTextures.end()) {

            LOG_ERROR("Attempting to get a animation texture that doesn't exist! name: {}", name_);
            return nullptr;
        }

        return m_AnimationTextures.at(name_).get();
    }

}
