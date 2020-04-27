#include "precomp.h"
#include "Texture.h"
#include "Render/Renderer.h"

#include <SDL.h>
#include <SDL_image.h>

namespace Engine
{
    bool Texture::LoadTexture(Renderer* renderer_, std::string path_)
    {
        if (m_Texture != nullptr)
        {
            LOG_WARNING("Overwriting already loaded texture with: {}", path_);
            SDL_DestroyTexture(m_Texture);
        }

        m_Texture = IMG_LoadTexture(renderer_->GetNativeRenderer(), path_.c_str());
        m_Shadow = nullptr;

        if (m_Texture == nullptr)
        {
            LOG_ERROR("Unable to load texture: {}, SDL_Image returned error {}", path_, IMG_GetError());
            return false;
        }

        return true;
    }

    bool Texture::LoadTexture(Renderer* renderer_, std::string path_, std::string pathShadow_)
    {
        if (m_Texture != nullptr)
        {
            LOG_WARNING("Overwriting already loaded texture with: {}", path_);
            SDL_DestroyTexture(m_Texture);
        }

        m_Texture = IMG_LoadTexture(renderer_->GetNativeRenderer(), path_.c_str());
        m_Shadow = IMG_LoadTexture(renderer_->GetNativeRenderer(), pathShadow_.c_str());

        if (m_Texture == nullptr)
        {
            LOG_ERROR("Unable to load texture: {}, SDL_Image returned error {}", path_, IMG_GetError());
            return false;
        }

        return true;
    }

    Texture::Texture(Renderer* renderer_, std::string path_)
    {
        LoadTexture(renderer_, path_);
    }

    Texture::Texture(Renderer* renderer_, std::string path_, std::string pathShadow_)
    {
        LoadTexture(renderer_, path_, pathShadow_);
    }

    Texture::~Texture()
    {
        if (m_Texture != nullptr)
        {
            SDL_DestroyTexture(m_Texture);
        }
        
        if (m_Shadow != nullptr) 
        {
            SDL_DestroyTexture(m_Shadow);
        }
    }

    bool AnimationTexture::LoadAnimationTexture(vec2 size_, int numOfParts_, Renderer* renderer_, std::string path_) {
        
        if (m_TextureSheet != nullptr)
        {
            LOG_WARNING("Overwriting already loaded texture with: {}", path_);
            SDL_DestroyTexture(m_TextureSheet);
        }

        m_TextureSheet = IMG_LoadTexture(renderer_->GetNativeRenderer(), path_.c_str());
        m_ShadowSheet = nullptr;
        
        m_Rects.reserve(numOfParts_);

        int w, h;
        SDL_QueryTexture(m_TextureSheet, NULL, NULL, &w, &h);
        int num_w = w / static_cast<int>(size_.x); //broj tekstura po horizontali
        int num_h = h / static_cast<int>(size_.y); //broj tekstura po vertikali

        //counter je izlazak iz petlje ako imamo recimo 7 delova teksture u matrici 3x3 (ne ucitavamo posled. dve)
        int counter = 0;
        for (int i = 0; i < num_h; i++) {
            for (int j = 0; j < num_w; j++) {

                m_Rects.push_back (SDL_Rect{
                    j * (int)size_.x,
                    i * (int)size_.y,
                    (int)size_.x,
                    (int)size_.y,
                });
                counter++;
                if (counter == numOfParts_) {
                    break;
                }
            }
        }

        if (m_TextureSheet == nullptr)
        {
            LOG_ERROR("Unable to load texture: {}, SDL_Image returned error {}", path_, IMG_GetError());
            return false;
        }

        return true;
    }
    bool AnimationTexture::LoadAnimationTexture(vec2 size_, int numOfParts_, Renderer* renderer_, std::string path_, std::string pathShadow_) {
        if (m_TextureSheet != nullptr)
        {
            LOG_WARNING("Overwriting already loaded texture with: {}", path_);
            SDL_DestroyTexture(m_TextureSheet);
        }

        m_TextureSheet = IMG_LoadTexture(renderer_->GetNativeRenderer(), path_.c_str());
        m_ShadowSheet = IMG_LoadTexture(renderer_->GetNativeRenderer(), pathShadow_.c_str());

        m_Rects.reserve(numOfParts_);

        int w, h;
        SDL_QueryTexture(m_TextureSheet, NULL, NULL, &w, &h);
        int num_w = w / static_cast<int>(size_.x); //broj tekstura po horizontali
        int num_h = h / static_cast<int>(size_.y); //broj tekstura po vertikali

        //counter je izlazak iz petlje ako imamo recimo 7 delova teksture u matrici 3x3 (ne ucitavamo posled. dve)
        int counter = 0;
        for (int i = 0; i < num_h; i++) {
            for (int j = 0; j < num_w; j++) {

                m_Rects[counter] = SDL_Rect{
                    j * (int)size_.x,
                    i * (int)size_.y,
                    (int)size_.x,
                    (int)size_.y,
                };
                counter++;
                if (counter == numOfParts_) {
                    break;
                }
            }
        }

        if (m_TextureSheet == nullptr)
        {
            LOG_ERROR("Unable to load texture: {}, SDL_Image returned error {}", path_, IMG_GetError());
            return false;
        }

        return true;
    }

    AnimationTexture::AnimationTexture(vec2 size_, int numOfParts_, Renderer* renderer_, std::string path_) {

        LoadAnimationTexture(size_, numOfParts_, renderer_, path_);
    }
    AnimationTexture::AnimationTexture(vec2 size_, int numOfParts_, Renderer* renderer_, std::string path_, std::string pathShadow_) {
        
        LoadAnimationTexture(size_, numOfParts_, renderer_, path_, pathShadow_);

    }
    AnimationTexture::~AnimationTexture() 
    {
        if (m_TextureSheet != nullptr)
        {
            SDL_DestroyTexture(m_TextureSheet);
        }

        if (m_ShadowSheet != nullptr)
        {
            SDL_DestroyTexture(m_ShadowSheet);
        }
    }
}
