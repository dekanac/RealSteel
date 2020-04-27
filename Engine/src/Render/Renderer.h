#pragma once
#include "Render/WindowData.h"

struct SDL_Renderer;

namespace Engine
{
#define SHADOW_POS_X 8.f
#define SHADOW_POS_Y 5.f

    struct WindowData;
    class Window;
    class Entity;

    class Renderer
    {
    public:
        bool Init(const WindowData& windowData_ = WindowData());
        bool Shutdown();

        void DrawTerrain(const std::vector<Entity*> renderables_, const Entity* camera);
        void DrawTerrainEntity(const Entity* t, const Entity* camera);
        void DrawEntities(const std::vector<Entity*> renderables_, const Entity* camera);
        void DrawEntity(const Entity* r, const Entity* camera);
        void DrawShadows(const std::vector<Entity*> shadows_, const Entity* camera);
        void DrawShadow(const Entity* s, const Entity* camera);
        void DrawAnimations(const std::vector<Entity*> animations_, const Entity* camera);
        void DrawAnimation(const Entity* a, const Entity* camera);
        void BeginScene() const;
        void EndScene() const;

        SDL_Renderer* GetNativeRenderer() const { return m_NativeRenderer; }

        void SetBackgroundColor(unsigned char bgR_, unsigned char bgG_, unsigned char bgB_, unsigned char bgA_);
        void SetBackgroundColor(const Color& col_);
        ~Renderer();

    private:
        std::unique_ptr<Window> m_Window;
        SDL_Renderer* m_NativeRenderer{ };
        Color m_BackgroundColor{ };
    };
}
