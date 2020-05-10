#include "precomp.h"
#include "PhysicsController.h"
#include "Entities/GameComponents.h"
#include "PhysicsController.h"
#include <Box2d.h>

namespace Game
{

    bool PhysicsController::Init(Engine::EntityManager* entityManager_)
    {
        ASSERT(entityManager_ != nullptr, "Must pass valid pointer to entitymanager to CameraController::Init()");

        auto controller = std::make_unique<Engine::Entity>();

        controller->AddComponent<Game::PhysicsComponent>();

        //Creating world and gravity for physics
        b2Vec2 gravity(0.0f, 0.0f);
        controller->GetComponent<Game::PhysicsComponent>()->world = new b2World(gravity);

        entityManager_->AddEntity(std::move(controller));
        return !(entityManager_->GetAllEntitiesWithComponent<Game::PhysicsComponent>().empty());
    }

    void PhysicsController::Update(float dt, Engine::EntityManager* entityManager_)
    {
        auto physicsComponent = entityManager_->GetAllComponentInstances<Game::PhysicsComponent>().back();
        physicsComponent->world->Step(0.02f, 6, 2);
    }
}
