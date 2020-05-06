#pragma once

namespace Engine
{
    class EntityManager;
}

namespace Game
{

    class CameraController
    {
    public:
        bool Init(Engine::EntityManager* entityManager_);
        void Update(float dt, Engine::EntityManager* entityManager_);
        void Shake();
    private:
        bool m_isShaking = false;
        float m_timePassed = 0.0f;
        float m_shakeDuration = 0.180f; //Trajanje u sekundama
        vec2 m_oldPosition{ DEFAULT_WIDTH / 2, DEFAULT_HEIGHT / 2 };
    };
}
