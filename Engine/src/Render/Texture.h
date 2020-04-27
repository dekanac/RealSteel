#pragma once


struct SDL_Texture;
struct SDL_Rect;

namespace Engine {

    class Renderer;

    struct Texture
    {
        SDL_Texture* m_Texture{};
        SDL_Texture* m_Shadow{};

        bool LoadTexture(Renderer* renderer_, std::string path_);
        bool LoadTexture(Renderer* renderer_, std::string path_, std::string pathShadow_);
        Texture() = default;
        Texture(Renderer* renderer_, std::string path_);
        Texture(Renderer* renderer_, std::string path_, std::string pathShadow_);
        ~Texture();
    };

    struct AnimationTexture 
    {
        SDL_Texture* m_TextureSheet{};
        SDL_Texture* m_ShadowSheet{};

        std::vector<SDL_Rect> m_Rects{};

        bool LoadAnimationTexture(vec2 size_, int numOfParts_, Renderer* renderer_, std::string path_);
        bool LoadAnimationTexture(vec2 size_, int numOfParts_, Renderer* renderer_, std::string path_, std::string pathShadow_);
        AnimationTexture() = default;
        AnimationTexture(vec2 size_, int numOfParts_, Renderer * renderer_, std::string path_);
        AnimationTexture(vec2 size_, int numOfParts_, Renderer * renderer_, std::string path_, std::string pathShadow_);
        ~AnimationTexture();
    };

}
