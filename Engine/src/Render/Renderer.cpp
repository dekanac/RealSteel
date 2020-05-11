#include "precomp.h"
#include "Renderer.h"
#include "Render/Window.h"
#include "Render/Texture.h"
#include "ECS/Entity.h"

#include <SDL.h>
#include <SDL_ttf.h>

namespace Engine
{

    bool Renderer::Init(const WindowData& windowData_)
    {
        LOG_INFO("Initializing Renderer");


        m_Window = std::make_unique<Window>();

        if (!m_Window->Init(windowData_))
        {
            LOG_CRITICAL("Unable to create a Window.");
        }

        m_NativeRenderer = SDL_CreateRenderer(
            m_Window->GetNativeWindowHandle(),
            -1,
            windowData_.m_Vsync ? SDL_RENDERER_PRESENTVSYNC : 0 |
            SDL_RENDERER_ACCELERATED);

        if (m_NativeRenderer == nullptr)
        {
            LOG_CRITICAL("Unable to create a renderer. SDL error: {}", SDL_GetError());
            return false;
        }

        SetBackgroundColor(100, 150, 236, SDL_ALPHA_OPAQUE);

        return true;
    }

    bool Renderer::Shutdown()
    {
        LOG_INFO("Shutting down Renderer");

        if (m_NativeRenderer != nullptr)
        {
            SDL_DestroyRenderer(m_NativeRenderer);
        }

        m_NativeRenderer = nullptr;

        m_Window.reset();

        return true;
    }

    vec2 GetScreenPosition(vec2 targetPosition, const Entity* camera)
    {
        vec2 screenCenter{ camera->GetComponent<TransformComponent>()->m_Size / 2.0f };
        return targetPosition - camera->GetComponent<TransformComponent>()->m_Position + screenCenter;
    }

    bool IsInsideScreen(vec2 targetWorldPosition, vec2 targetSize, const Entity* camera)
    {
        vec2 screenPosition = GetScreenPosition(targetWorldPosition, camera);
        return (screenPosition.x + targetSize.x / 2.0f >= 0 && screenPosition.x - targetSize.x / 2.0f <= camera->GetComponent<TransformComponent>()->m_Size.x)
            && (screenPosition.y + targetSize.y / 2.0f >= 0 && screenPosition.y - targetSize.y / 2.0f <= camera->GetComponent<TransformComponent>()->m_Size.y);
    }

    void Renderer::DrawEntities(const std::vector<Entity*> renderables_, const Entity* camera)
    {
        for (const auto r : renderables_)
        {
            DrawEntity(r, camera);
        }
    }

    void Renderer::DrawEntity(const Entity* r, const Entity* camera)
    {
        auto transform = r->GetComponent<TransformComponent>();
        auto sprite = r->GetComponent<SpriteComponent>();

        vec2 size = transform->m_Size;
        if (size == vec2{ 0.f, 0.f }) // Use size of texture if size of entity isn't set
        {
            int w, h;
            SDL_QueryTexture(sprite->m_Image->m_Texture, NULL, NULL, &w, &h);
            size.x = static_cast<float>(w);
            size.y = static_cast<float>(h);
        }

        if (IsInsideScreen(transform->m_Position, vec2(size.x, size.y), camera))
        {
            vec2 screenPosition = GetScreenPosition(transform->m_Position, camera);
            SDL_Rect dst{ (int)(screenPosition.x - size.x / 2), (int)(screenPosition.y - size.y / 2), (int)size.x, (int)size.y };
            SDL_RendererFlip flip = static_cast<SDL_RendererFlip>((sprite->m_FlipHorizontal ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE) | (sprite->m_FlipVertical ? SDL_FLIP_VERTICAL : SDL_FLIP_NONE));

            SDL_RenderCopyEx(
                m_NativeRenderer,
                sprite->m_Image->m_Texture,
                NULL,
                &dst,
                transform->m_Rotation,
                NULL,
                flip);

#ifdef _DEBUG 
            // DebugDraw
            SDL_SetRenderDrawColor(m_NativeRenderer, 255, 0, 0, SDL_ALPHA_OPAQUE);

            SDL_RenderDrawPoint(m_NativeRenderer, (int)screenPosition.x, (int)screenPosition.y);

            if (auto collider = r->GetComponent<CollisionComponent>())
            {
                SDL_SetRenderDrawColor(m_NativeRenderer, 255, 0, 0, SDL_ALPHA_OPAQUE);
                dst = { (int)(screenPosition.x - collider->m_Size.x / 2), (int)(screenPosition.y - collider->m_Size.y / 2), (int)collider->m_Size.x, (int)collider->m_Size.y };
                SDL_RenderDrawRect(m_NativeRenderer, &dst);

            }
            
            SetBackgroundColor(m_BackgroundColor);
#endif
        }
    }

    void Renderer::DrawAnimations(const std::vector<Entity*> animations_, const Entity* camera)
    {
        for (const auto r : animations_)
        {
            DrawAnimation(r, camera);
        }
    }

    void Renderer::DrawAnimation(const Entity* a, const Entity* camera) 
    {
        
        auto transform = a->GetComponent<TransformComponent>();
        auto animation = a->GetComponent<AnimationComponent>();

        if (animation->m_CurrentRectToDraw == -1) {
            return;
        }

        vec2 size = transform->m_Size;
        ASSERT ((size != vec2{ 0.f, 0.f }), "Animation's transform component must have size initialized");

        if (IsInsideScreen(transform->m_Position, vec2(size.x, size.y), camera))
        {
            
            vec2 screenPosition = GetScreenPosition(transform->m_Position, camera);
            SDL_Rect dst{ (int)(screenPosition.x - size.x / 2), (int)(screenPosition.y - size.y / 2), (int)size.x, (int)size.y };
            SDL_RendererFlip flip = SDL_FLIP_NONE;
            
            SDL_RenderCopyEx(
                m_NativeRenderer,
                animation->m_TextureSheet->m_TextureSheet,
                &animation->m_TextureSheet->m_Rects[animation->m_CurrentRectToDraw],
                &dst,
                transform->m_Rotation,
                NULL,
                flip);
            
           
        }
        
    }

    void Renderer::DrawScore(const Entity* s, const Entity* camera)
    {
        auto transform = s->GetComponent<TransformComponent>();
        auto score = s->GetComponent<ScoreComponent>()->score;

        SDL_Color color = { 255, 255, 255 };
        SDL_Surface* score_display = TTF_RenderText_Solid(font, score.c_str(), color);
        SDL_Texture* texture = SDL_CreateTextureFromSurface(m_NativeRenderer, score_display);

        int x = 0;
        int y = 0;
        SDL_QueryTexture(texture, NULL, NULL, &x, &y);
        SDL_Rect rect = { 10, 10, x, y };

        SDL_RenderCopyEx(
            m_NativeRenderer,
            texture,
            NULL,
            &rect,
            NULL,
            NULL,
            SDL_FLIP_NONE);

        SDL_DestroyTexture(texture);
        SDL_FreeSurface(score_display);
    }

    void Renderer::DrawShadows(const std::vector<Entity*> shadows_, const Entity* camera)
    {
        for (const auto s : shadows_)
        {
            DrawShadow(s, camera);
        }
    }

    void Renderer::DrawShadow(const Entity* s, const Entity* camera)
    {
        auto transform = s->GetComponent<TransformComponent>();
        auto sprite = s->GetComponent<SpriteComponent>();

        //ASSERT((sprite->m_Image->m_Shadow != nullptr), "Shadow texture doesn't exist!");
        if (sprite->m_Image->m_Shadow != nullptr)
        {
            return;
        }

        vec2 size = transform->m_Size;
        if (size == vec2{ 0.f, 0.f }) // Use size of texture if size of entity isn't set
        {
            int w, h;
            SDL_QueryTexture(sprite->m_Image->m_Shadow, NULL, NULL, &w, &h);
            size.x = static_cast<float>(w);
            size.y = static_cast<float>(h);
        }

        vec2 shadowPosition = vec2{ transform->m_Position.x + SHADOW_POS_X, transform->m_Position.y + SHADOW_POS_Y };

        if (IsInsideScreen(shadowPosition, vec2(size.x, size.y), camera))
        {
            vec2 screenPosition = GetScreenPosition(shadowPosition, camera);
            SDL_Rect dst{ (int)(screenPosition.x - size.x / 2), (int)(screenPosition.y - size.y / 2), (int)size.x, (int)size.y };
            SDL_RendererFlip flip = static_cast<SDL_RendererFlip>((sprite->m_FlipHorizontal ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE) | (sprite->m_FlipVertical ? SDL_FLIP_VERTICAL : SDL_FLIP_NONE));

            SDL_RenderCopyEx(
                m_NativeRenderer,
                sprite->m_Image->m_Shadow,
                NULL,
                &dst,
                transform->m_Rotation,
                NULL,
                flip);
        }
    }

    void Renderer::DrawTerrain(const std::vector<Entity*> terrain_, const Entity* camera)
    {
        for (const auto t : terrain_)
        {
            DrawTerrainEntity(t, camera);
        }
    }

    void Renderer::DrawTerrainEntity(const Entity* t, const Entity* camera)
    {
        auto transform = t->GetComponent<TransformComponent>();
        auto sprite = t->GetComponent<TerrainComponent>();

        vec2 size = transform->m_Size;
        if (size == vec2{ 0.f, 0.f }) // Use size of texture if size of entity isn't set
        {
            int w, h;
            SDL_QueryTexture(sprite->m_Texture->m_Texture, NULL, NULL, &w, &h);
            size.x = static_cast<float>(w);
            size.y = static_cast<float>(h);
        }

        if (IsInsideScreen(transform->m_Position, vec2(size.x, size.y), camera))
        {
            vec2 screenPosition = GetScreenPosition(transform->m_Position, camera);
            SDL_Rect dst{ (int)(screenPosition.x - size.x / 2), (int)(screenPosition.y - size.y / 2), (int)size.x, (int)size.y };
            SDL_RendererFlip flip = static_cast<SDL_RendererFlip>((sprite->m_FlipHorizontal ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE) | (sprite->m_FlipVertical ? SDL_FLIP_VERTICAL : SDL_FLIP_NONE));

            SDL_RenderCopyEx(
                m_NativeRenderer,
                sprite->m_Texture->m_Texture,
                NULL,
                &dst,
                transform->m_Rotation,
                NULL,
                flip);
        }
    }

    void Renderer::SetBackgroundColor(unsigned char bgR_, unsigned char bgG_, unsigned char bgB_, unsigned char bgA_)
    {
        m_BackgroundColor.r = bgR_;
        m_BackgroundColor.g = bgG_;
        m_BackgroundColor.b = bgB_;
        m_BackgroundColor.a = bgA_;
        SDL_SetRenderDrawColor(m_NativeRenderer, bgR_, bgG_, bgB_, bgA_);
    }

    void Renderer::SetBackgroundColor(const Color& col_)
    {
        m_BackgroundColor = col_;
        SDL_SetRenderDrawColor(m_NativeRenderer, m_BackgroundColor.r, m_BackgroundColor.g, m_BackgroundColor.b, m_BackgroundColor.a);
    }

    void Renderer::BeginScene() const
    {
        SDL_RenderClear(m_NativeRenderer);
    }

    void Renderer::EndScene() const
    {
        SDL_RenderPresent(m_NativeRenderer);
    }

    Renderer::~Renderer()
    {
        Shutdown();
    }

}
