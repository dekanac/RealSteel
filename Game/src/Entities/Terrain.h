#pragma once

namespace Engine
{
    class EntityManager;
    struct Texture;
}

namespace Game
{

    class Terrain
    {
    public:
        bool Init(Engine::EntityManager* entityManager_, Engine::TextureManager* textureManager_);
        void Update(float dt, Engine::EntityManager* em_);
    private:
        bool CreateLake(float rotation_, vec2 pos_, Engine::EntityManager* entityManager_, Engine::TextureManager* textureManager_);

    };
}
