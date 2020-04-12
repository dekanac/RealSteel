#include "precomp.h"
#include "Player.h"

#include <cmath>

namespace Game {

    bool Player::Init(Engine::EntityManager* entityManager_, Engine::Texture* texture_) {

        ASSERT(entityManager_ != nullptr, "Must pass valid pointer to entitymanager");
        ASSERT(texture_ != nullptr, "Must pass valid pointer to texture");

        auto player = std::make_unique<Engine::Entity>();

        player->AddComponent<Engine::PlayerComponent>();
        player->AddComponent<Engine::TransformComponent>(m_StartPos[0], m_StartPos[1], 30.f, 40.f);
        player->AddComponent<Engine::CollisionComponent>(30.f, 30.f);
        player->AddComponent<Engine::HealthComponent>();
        player->AddComponent<Engine::MoverComponent>();
        player->AddComponent<Engine::InputComponent>();
        player->AddComponent<Engine::SpriteComponent>().m_Image = texture_;

        auto health = player->GetComponent<Engine::HealthComponent>();
        health->m_MaxHealth = 100;
        health->m_CurrentHealth = health->m_MaxHealth;
        
        auto inputComp = player->GetComponent<Engine::InputComponent>();

        inputComp->inputActions.push_back({ fmt::format("Player{}MoveUp", m_Player) });
        inputComp->inputActions.push_back({ fmt::format("Player{}MoveDown", m_Player) });
        inputComp->inputActions.push_back({ fmt::format("Player{}MoveLeft", m_Player) });
        inputComp->inputActions.push_back({ fmt::format("Player{}MoveRight", m_Player) });

        entityManager_->AddEntity(std::move(player));

        return true;
    }

    void Player::Update(float dt, Engine::EntityManager* entityManager_) {

        auto entitiesToMove = entityManager_->GetAllEntitiesWithComponents<Engine::PlayerComponent, Engine::MoverComponent, Engine::InputComponent>();
        auto entity = entitiesToMove[m_Player - 1];
        auto move = entity->GetComponent<Engine::MoverComponent>();
        auto input = entity->GetComponent<Engine::InputComponent>();
        auto speed = m_Speed;

        bool moveUpInput = Engine::InputManager::IsActionActive(input, fmt::format("Player{}MoveUp", m_Player));
        bool moveDownInput = Engine::InputManager::IsActionActive(input, fmt::format("Player{}MoveDown", m_Player));
        bool moveRightInput = Engine::InputManager::IsActionActive(input, fmt::format("Player{}MoveRight", m_Player));
        bool moveLeftInput = Engine::InputManager::IsActionActive(input, fmt::format("Player{}MoveLeft", m_Player));
        //rotation in degrees
        auto rotation_deg = entity->GetComponent<Engine::TransformComponent>()->m_Rotation;
        //rotation in radians
        float rotation = (rotation_deg * (float)M_PI) / 180.f;

        move->m_RotationSpeed = speed * ((moveLeftInput ? -1.0f : 0.0f) + (moveRightInput ? 1.0f : 0.0f));
        move->m_TranslationSpeed.y = speed * ((moveDownInput ? cos(rotation) : 0.0f) + (moveUpInput ? -cos(rotation) : 0.0f));
        move->m_TranslationSpeed.x = speed * ((moveDownInput ? -sin(rotation) : 0.0f) + (moveUpInput ? sin(rotation) : 0.0f));

    }

    void Player::SetSpeed(float speed_) {

        m_Speed = speed_;
    }

    Player::Player(int player_, vec2 startPos_)
        : m_Player(player_), m_StartPos(startPos_) {}
    Player::Player(int player_, vec2 startPos_, float speed_)
        : m_Player(player_), m_StartPos(startPos_), m_Speed(speed_) {}

}