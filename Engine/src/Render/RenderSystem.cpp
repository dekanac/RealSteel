#include "precomp.h"
#include "RenderSystem.h"
#include "Render/Renderer.h"
#include "Render/Window.h"
#include "ECS/EntityManager.h"
#include "ECS/Component.h"

#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>


namespace Engine
{
    bool RenderSystem::Init(const WindowData& windowData_)
    {
        LOG_INFO("Initializing RenderSystem");


        if (SDL_Init(SDL_INIT_VIDEO) < 0)
        {
            LOG_CRITICAL("Unable to initialize SDL. SDL error: {}", SDL_GetError());
            return false;
        }

        if(TTF_Init() != 0)
        {
            LOG_CRITICAL("Unable to initialize TTF.");
            return false;
        }

        if (IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG) < 0)
        {
            LOG_CRITICAL("Unable to initialize SDL_Image");
            return false;
        }

        m_Renderer = std::make_unique<Renderer>();

        if (!m_Renderer->Init(windowData_))
        {
            LOG_CRITICAL("Unable to initialize renderer");
            return false;
        }

        m_Renderer->Init(windowData_);

        LOG_INFO("RenderSystem initialized successfully");
        return true;
    }

    bool RenderSystem::Shutdown()
    {
        LOG_INFO("Shutting down RenderSystem");

        m_Renderer.reset();
        SDL_Quit();
        TTF_Quit();

        return true;
    }

    void RenderSystem::Update(float dt_, EntityManager* entityManager)
    {
        m_Renderer->BeginScene();

        // Get the main camera from the entity manager
        // TODO: Support multiple cameras and switching between them
        auto cameras = entityManager->GetAllEntitiesWithComponents<CameraComponent, TransformComponent>();
        ASSERT(!cameras.empty(), "Must have at least one camera");

        auto camera = *(cameras.begin());
        //draw shadows
        auto terrain = entityManager->GetAllEntitiesWithComponents<TransformComponent, TerrainComponent>();
        auto shadows = entityManager->GetAllEntitiesWithComponents<TransformComponent , SpriteComponent, ShadowComponent>();
        auto renderables = entityManager->GetAllEntitiesWithComponents<TransformComponent, SpriteComponent>();
        auto animations = entityManager->GetAllEntitiesWithComponents<TransformComponent, AnimationComponent>();
        auto scores = entityManager->GetAllEntitiesWithComponent< ScoreComponent>();
        Entity* score = NULL;
        if (scores.size() != 0)
            score = scores.at(0);

        
        m_Renderer->DrawTerrain(terrain, camera);
        m_Renderer->DrawShadows(shadows, camera);
        m_Renderer->DrawEntities(renderables, camera);
        m_Renderer->DrawAnimations(animations, camera);
        if (scores.size() != 0)
        {
            m_Renderer->DrawScore(score, camera);
        }

        m_Renderer->EndScene();
    }

    Renderer* RenderSystem::GetRenderer()
    {
        return m_Renderer.get();
    }

    void RenderSystem::SetBackgroundColor(unsigned char bgR_, unsigned char bgG_, unsigned char bgB_, unsigned char bgA_)
    {
        m_Renderer->SetBackgroundColor(bgR_, bgG_, bgB_, bgA_);
    }

    void RenderSystem::SetBackgroundColor(const Color& col_)
    {
        m_Renderer->SetBackgroundColor(col_);
    }

}
