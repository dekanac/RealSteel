#include "precomp.h"
#include "GameComponents.h"
#include "Player.h"
#include "Tank.h"


#include <cmath>

namespace Game {

    bool Player::AddPlayer(vec2 startPos_, Engine::EntityManager* entityManager_, Engine::TextureManager* textureManager_) {

        ASSERT(entityManager_ != nullptr, "Must pass valid pointer to entitymanager");
        ASSERT(textureManager_ != nullptr, "Must pass valid pointer to texture");

        m_Player++;
        auto player = std::make_unique<Engine::Entity>();

        player->AddComponent<Game::PlayerGameComponent>();
        player->AddComponent<Engine::InputComponent>();

        auto playersTank = std::make_unique<Tank>();
        auto playersTankEntity = playersTank->CreateTank(startPos_, entityManager_, textureManager_);
        player->GetComponent<Game::PlayerGameComponent>()->tankEntity = std::move(playersTankEntity);

        auto inputComp = player->GetComponent<Engine::InputComponent>();

        inputComp->inputActions.push_back({ fmt::format("Player{}MoveUp", m_Player) });
        inputComp->inputActions.push_back({ fmt::format("Player{}MoveDown", m_Player) });
        inputComp->inputActions.push_back({ fmt::format("Player{}MoveLeft", m_Player) });
        inputComp->inputActions.push_back({ fmt::format("Player{}MoveRight", m_Player) });
        inputComp->inputActions.push_back({ fmt::format("Player{}Fire", m_Player) });
        //inputComp->inputActions.push_back({ fmt::format("Player{}RotateTurretCW", m_Player) });
        //inputComp->inputActions.push_back({ fmt::format("Player{}RotateTurretCCW", m_Player) });

        entityManager_->AddEntity(std::move(player));

        return true;
    }

    void Player::Update(float dt, Engine::EntityManager* entityManager_) {

        auto playerEntities = entityManager_->GetAllEntitiesWithComponent<Game::PlayerGameComponent>();
        
        int i = 1;
        for (auto& player : playerEntities) {
            auto playersTank = player->GetComponent<Game::PlayerGameComponent>()->tankEntity;
            ASSERT(playersTank != nullptr, "players tank not initialized!");

            auto playersTankTurret = playersTank->GetComponent<Game::TankComponent>()->tankTurretEntity;

            auto move = playersTank->GetComponent<Engine::MoverComponent>();
            auto input = player->GetComponent<Engine::InputComponent>();
            auto speed = playersTank->GetComponent<Game::TankComponent>()->tankSpeed;
            auto tankTransf = playersTank->GetComponent<Engine::TransformComponent>();

            bool moveUpInput = Engine::InputManager::IsActionActive(input, fmt::format("Player{}MoveUp", i));
            bool moveDownInput = Engine::InputManager::IsActionActive(input, fmt::format("Player{}MoveDown", i));
            bool moveRightInput = Engine::InputManager::IsActionActive(input, fmt::format("Player{}MoveRight", i));
            bool moveLeftInput = Engine::InputManager::IsActionActive(input, fmt::format("Player{}MoveLeft", i));

            auto payersTransformComponent = playersTank->GetComponent<Engine::TransformComponent>();
            auto rotationDeg = payersTransformComponent->m_Rotation;
            float rotationRad = (rotationDeg * (float)M_PI) / 180.f;

            move->m_RotationSpeed = speed * ((moveLeftInput ? -1.0f : 0.0f) + (moveRightInput ? 1.0f : 0.0f));
            move->m_TranslationSpeed.y = speed * ((moveDownInput ? cos(rotationRad) : 0.0f) + (moveUpInput ? -cos(rotationRad) : 0.0f));
            move->m_TranslationSpeed.x = speed * ((moveDownInput ? -sin(rotationRad) : 0.0f) + (moveUpInput ? sin(rotationRad) : 0.0f));
            
            //posebna rotacija za turret koja prati poziciju misa
            auto turretTransf = playersTankTurret->GetComponent<Engine::TransformComponent>();
            vec2 direction = vec2{ input->mouse_x, input->mouse_y } - tankTransf->m_Position;
            float angle = atan2(direction.x, direction.y) * (180.f / 3.14f);
            //malo tvikovanja i radi :D
            turretTransf->m_Rotation = angle * (-1.f) + 180.f;

            i++;
        }
    }
}