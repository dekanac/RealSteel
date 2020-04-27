#include "precomp.h"

#include "Entities/Terrain.h"
#include "GameComponents.h"

namespace Game
{
    bool Terrain::Init (Engine::EntityManager* entityManager_, Engine::TextureManager* textureManager_)
    {
        ASSERT(textureManager_ != nullptr, "nullptr passed as textureManager_");
        ASSERT(entityManager_ != nullptr, "Must pass valid pointer to entitymanager to CameraController::Init()");

        auto imageEntity = std::make_unique<Engine::Entity>();
        imageEntity->AddComponent<Engine::TransformComponent>(DEFAULT_WIDTH/2, DEFAULT_HEIGHT/2, 1280.f, 720.f);
        imageEntity->AddComponent<Engine::TerrainComponent>().m_Texture = textureManager_->GetTexture("terrain");
        
        entityManager_->AddEntity(std::move(imageEntity));

        CreateLake(0.f, vec2{ 640.f, 185.f }, entityManager_, textureManager_);
        CreateLake(127.f, vec2{ 320.f, 540.f }, entityManager_, textureManager_);


        return true;
    }

    bool Terrain::CreateLake(float rotation_, vec2 pos_, Engine::EntityManager* entityManager_, Engine::TextureManager* textureManager_) {

        auto lake = std::make_unique<Engine::Entity>();
        lake->AddComponent<Engine::TransformComponent>();

        auto lakeTransf = lake->GetComponent<Engine::TransformComponent>();
        lakeTransf->m_Position = pos_;
        lakeTransf->m_Rotation = rotation_;

        //za potrebe collision boxa crtati sa sprite-om
        //lake->AddComponent<Engine::SpriteComponent>(textureManager_->GetTexture("lake"));
        lake->AddComponent<Engine::CollisionComponent>(120.f, 120.f);
        
        lake->AddComponent<Engine::TerrainComponent>().m_Texture = textureManager_->GetTexture("lake");
        lake->AddComponent<Game::LakeComponent>();

        entityManager_->AddEntity(std::move(lake));

        return true;

    }

    void Terrain::Update(float dt, Engine::EntityManager* em_) {

    }
}
