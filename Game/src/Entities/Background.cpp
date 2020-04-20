#include "precomp.h"

#include "Entities/Background.h"

namespace Game
{
    bool Background::Init (Engine::EntityManager* entityManager_, Engine::TextureManager* textureManager_)
    {
        ASSERT(textureManager_ != nullptr, "nullptr passed as textureManager_");
        ASSERT(entityManager_ != nullptr, "Must pass valid pointer to entitymanager to CameraController::Init()");

        auto imageEntity = std::make_unique<Engine::Entity>();
        imageEntity->AddComponent<Engine::TransformComponent>(DEFAULT_WIDTH/2, DEFAULT_HEIGHT/2, 1280.f, 720.f);
        imageEntity->AddComponent<Engine::TerrainComponent>().m_Texture = textureManager_->GetTexture("terrain");
        
        entityManager_->AddEntity(std::move(imageEntity));

        return true;
    }

    void Background::Update(float dt)
    {

    }
}
