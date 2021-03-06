#include "precomp.h"
#include "CameraController.h"
#include <random>


namespace Game
{

    bool CameraController::Init(Engine::EntityManager* entityManager_)
    {
        ASSERT(entityManager_ != nullptr, "Must pass valid pointer to entitymanager to CameraController::Init()");

        auto camera = std::make_unique<Engine::Entity>();
        //lakse je kad koordinate krecu od (0,0) nego da ima tacaka u sva 4 kvadranta, ovako su sve pozitivne
        camera->AddComponent<Engine::TransformComponent>(DEFAULT_WIDTH/2, DEFAULT_HEIGHT/2);
        camera->AddComponent<Engine::CameraComponent>(100.f);
        camera->AddComponent<Engine::InputComponent>();
        camera->AddComponent<Engine::MoverComponent>();

        auto transformComp = camera->GetComponent<Engine::TransformComponent>();
        transformComp->m_Size = { 1280.f, 720.f };

        auto inputComp = camera->GetComponent<Engine::InputComponent>();

        //TODO:Kamera je staticna, Mozda dozvoliti u nekom debug modu da se pomera

        inputComp->inputActions.push_back({ "PanCameraUp" });
        inputComp->inputActions.push_back({ "PanCameraDown" });
        inputComp->inputActions.push_back({ "PanCameraRight" });
        inputComp->inputActions.push_back({ "PanCameraLeft" });

        entityManager_->AddEntity(std::move(camera));

        return !(entityManager_->GetAllEntitiesWithComponent<Engine::CameraComponent>().empty());
    }

    void CameraController::Update(float dt, Engine::EntityManager* entityManager_)
    {
        auto entitiesToMove = entityManager_->GetAllEntitiesWithComponents<Engine::CameraComponent, Engine::MoverComponent, Engine::InputComponent>();

        for (auto& entity : entitiesToMove)
        {
            auto move = entity->GetComponent<Engine::MoverComponent>();
            auto transform = entity->GetComponent<Engine::TransformComponent>();
            auto input = entity->GetComponent<Engine::InputComponent>();
            auto speed = entity->GetComponent<Engine::CameraComponent>()->m_PanSpeed;

            //TODO:Kamera je staticna, Mozda dozvoliti u nekom debug modu da se pomera
            bool moveUpInput = Engine::InputManager::IsActionActive(input, "PanCameraUp");
            bool moveDownInput = Engine::InputManager::IsActionActive(input, "PanCameraDown");
            bool moveLeftInput = Engine::InputManager::IsActionActive(input, "PanCameraLeft");
            bool moveRightInput = Engine::InputManager::IsActionActive(input, "PanCameraRight");

            move->m_TranslationSpeed.x = speed * ((moveLeftInput ? -1.0f : 0.0f) + (moveRightInput ? 1.0f : 0.0f));
            move->m_TranslationSpeed.y = speed * ((moveUpInput ? -1.0f : 0.0f) + (moveDownInput ? 1.0f : 0.0f));

            if (m_isShaking) {
                std::random_device rd;
                std::mt19937 gen(rd());
                std::uniform_real_distribution<> dis(0.0f, 2.2f); 
                move->m_TranslationSpeed.x = speed * static_cast<float>(dis(gen)-1.1f); //Generisanje random broja izmedju -1.1 i 1.1
                move->m_TranslationSpeed.y = speed * static_cast<float>(dis(gen)-1.1f);
                m_timePassed += dt;

                if (m_timePassed >= m_shakeDuration) {
                    transform->m_Position = m_oldPosition;
                    m_isShaking = false;
                    m_timePassed = 0;
                }
            }
        }
    }
    void CameraController::Shake()
    {
        m_isShaking = true;
    }
}
