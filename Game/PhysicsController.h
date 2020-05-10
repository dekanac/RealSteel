#pragma once
#include <Box2d.h>

namespace Engine
{
    class EntityManager;
}

namespace Game
{

    class PhysicsController
    {
    public:
        bool Init(Engine::EntityManager* entityManager_);
        void Update(float dt, Engine::EntityManager* entityManager_);
       
    private:
    };
}
#pragma once
