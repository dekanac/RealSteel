#include "precomp.h"

#include "Entities/Background.h"

namespace Game
{
    bool Background::Init (Engine::EntityManager* entityManager_, Engine::Texture* texture_)
    {
        ASSERT(texture_ != nullptr, "nullptr passed as texture_");
        ASSERT(entityManager_ != nullptr, "Must pass valid pointer to entitymanager to CameraController::Init()");

        auto imageEntity = std::make_unique<Engine::Entity>();
        imageEntity->AddComponent<Engine::TransformComponent>(0.f, 0.f,1280.f, 720.f);
        imageEntity->AddComponent<Engine::SpriteComponent>().m_Image = texture_;
        
        entityManager_->AddEntity(std::move(imageEntity));

        return true;
    }

    void Background::Update(float dt)
    {

    }
}
