#pragma once


struct SDL_Texture;

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
}
