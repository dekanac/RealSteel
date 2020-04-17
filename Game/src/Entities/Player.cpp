#include "precomp.h"
#include "GameComponents.h"
#include "Player.h"
#include "Tank.h"


#include <cmath>

namespace Game {

    bool Player::Init(Engine::EntityManager* entityManager_, Engine::TextureManager* textureManager_) {

        ASSERT(entityManager_ != nullptr, "Must pass valid pointer to entitymanager");
        ASSERT(textureManager_ != nullptr, "Must pass valid pointer to texture");

        auto player = std::make_unique<Engine::Entity>();

        player->AddComponent<Game::PlayerGameComponent>();
        player->AddComponent<Engine::InputComponent>();

        auto playersTank = std::make_unique<Tank>();
        auto playersTankEntity = playersTank->CreateTank(m_StartPos, entityManager_, textureManager_);
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

            //auto playersTankTurret = playersTank->GetComponent<Game::TankComponent>()->tankTurretEntity;

            auto move = playersTank->GetComponent<Engine::MoverComponent>();
            auto input = player->GetComponent<Engine::InputComponent>();
            auto speed = playersTank->GetComponent<Game::TankComponent>()->tankSpeed;

            bool moveUpInput = Engine::InputManager::IsActionActive(input, fmt::format("Player{}MoveUp", i));
            bool moveDownInput = Engine::InputManager::IsActionActive(input, fmt::format("Player{}MoveDown", i));
            bool moveRightInput = Engine::InputManager::IsActionActive(input, fmt::format("Player{}MoveRight", i));
            bool moveLeftInput = Engine::InputManager::IsActionActive(input, fmt::format("Player{}MoveLeft", i));
            //bool rotateTurretCW = Engine::InputManager::IsActionActive(input, fmt::format("Player{}RotateTurretCW", m_Player));
            //bool rotateTurretCCW = Engine::InputManager::IsActionActive(input, fmt::format("Player{}RotateTurretCCW", m_Player));

            //rotation in degrees
            auto payersTransformComponent = playersTank->GetComponent<Engine::TransformComponent>();
            auto rotation_deg = payersTransformComponent->m_Rotation;
            //rotation in radians
            float rotation = (rotation_deg * (float)M_PI) / 180.f;
            //auto turret_move = playersTankTurret->GetComponent<Engine::MoverComponent>();

            move->m_RotationSpeed = speed * ((moveLeftInput ? -1.0f : 0.0f) + (moveRightInput ? 1.0f : 0.0f));
            move->m_TranslationSpeed.y = speed * ((moveDownInput ? cos(rotation) : 0.0f) + (moveUpInput ? -cos(rotation) : 0.0f));
            move->m_TranslationSpeed.x = speed * ((moveDownInput ? -sin(rotation) : 0.0f) + (moveUpInput ? sin(rotation) : 0.0f));
            //turret_move->m_RotationSpeed = speed * ((rotateTurretCW ? 1.0f : 0.0f) + (rotateTurretCCW ? -1.0f : 0.0f));
            i++;
        }
    }


    Player::Player(int player_, vec2 startPos_)
        : m_Player(player_), m_StartPos(startPos_) {}

}