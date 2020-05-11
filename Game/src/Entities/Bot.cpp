#include "precomp.h"
#include "src/Entities/GameComponents.h"
#include "src/Entities/GridSystem.h"
#include "src/Entities/Bot.h"
#include "src/Entities/Tank.h"

namespace Game 

{
    bool Game::Bot::AddBot(vec2 startPos_, Engine::EntityManager* entityManager_, Engine::TextureManager* textureManager_)
    {
        ASSERT(entityManager_ != nullptr, "Must pass valid pointer to entitymanager");
        ASSERT(textureManager_ != nullptr, "Must pass valid pointer to texture");

        m_time = 0.f;
        m_startingPosition = startPos_;
        m_newPosition = startPos_;

        auto bot = std::make_unique<Engine::Entity>();

        bot->AddComponent<Game::BotComponent>();
        bot->AddComponent<Engine::InputComponent>();

        auto botsTank = std::make_unique<Tank>();
        auto botsTankEntity = botsTank->CreateTank(m_startingPosition, entityManager_, textureManager_);
        bot->GetComponent<Game::BotComponent>()->tankEntity = std::move(botsTankEntity);

        entityManager_->AddEntity(std::move(bot));

        return true;
    }

    void Game::Bot::Update(float dt, Engine::EntityManager* entityManager_, Game::GridSystem* gridSystem_, Engine::SoundManager* soundManager_, Engine::TextureManager* textureManager_)
    {
        auto player = entityManager_->GetAllEntitiesWithComponent<Game::PlayerGameComponent>()[0];
        auto playerTransfComp = player->GetComponent<Game::PlayerGameComponent>()->tankEntity->GetComponent<Engine::TransformComponent>();

        auto bots = entityManager_->GetAllEntitiesWithComponent<Game::BotComponent>();

        m_time += dt;


        for (auto& bot : bots) {
            auto botTank = bot->GetComponent<Game::BotComponent>()->tankEntity;
            ASSERT(botTank != nullptr, "players tank not initialized!");

            bool rotateLeft = false, rotateRight = false;
            bool moveUp = false, moveDown = false;

            auto playersTankTurret = botTank->GetComponent<Game::TankComponent>()->tankTurretEntity;

            auto move = botTank->GetComponent<Engine::MoverComponent>();
            auto speed = botTank->GetComponent<Game::TankComponent>()->tankSpeed;
            auto tankTransf = botTank->GetComponent<Engine::TransformComponent>();
            std::vector<int> botGridCoordinates = gridSystem_->transformToGridCoordinates(tankTransf->m_Position[0], tankTransf->m_Position[1]);
            std::vector<int> playerGridCoordinates = gridSystem_->transformToGridCoordinates(playerTransfComp->m_Position[0], playerTransfComp->m_Position[1]);

            if (m_time >= 0.2f) {
                float distanceToPlayer = distanceToPLayer(tankTransf->m_Position, playerTransfComp->m_Position);
                if (distanceToPlayer > 100.f) {
                    //std::cout << "Bot:" << botGridCoordinates[0] << ", " << botGridCoordinates[1] << std::endl;
                    //std::cout << "Player: " << playerGridCoordinates[0] << ", " << playerGridCoordinates[1] << std::endl;
                    m_newPosition = gridSystem_->BFS(botGridCoordinates, playerGridCoordinates);
                    bot->GetComponent<Game::BotComponent>()->m_isMoving = true;
                    m_time = 0.f;
                }
                else {
                    bot->GetComponent<Game::BotComponent>()->m_isMoving = false;
                }
            }

            if (true)
            {
                Tank::Shoot(botTank, true, entityManager_, soundManager_, textureManager_, false,
                    botTank->GetComponent<Game::TankComponent>()->missilePower);
            }


            if (tankTransf->m_Rotation <= -360.0f) {
                tankTransf->m_Rotation += 360.0f;
            }
            else if (tankTransf->m_Rotation >= 360.0f) {
                tankTransf->m_Rotation -= 360.0f;
            }
            auto rotationDeg = tankTransf->m_Rotation;
            float rotationRad = (rotationDeg * 3.14f) / 180.f;


            //orijentisanje bota ka novoj poziciji
            vec2 orientationDirection = tankTransf->m_Position - m_newPosition;
            float orientationAngleDeg = std::fmod(atan2(orientationDirection.x, orientationDirection.y) * (-180.f / 3.14f), 360.f);
            float orientationAngleRad = (orientationAngleDeg * 3.14f) / 180.f;


            if (orientationAngleDeg > rotationDeg) {
                if (std::fabs(std::fabs(orientationAngleDeg) - std::fabs(rotationDeg)) > 2.f) {
                    rotateRight = true;
                }
            }
            else if (orientationAngleDeg < rotationDeg) {
                if (std::fabs(std::fabs(orientationAngleDeg) - std::fabs(rotationDeg)) > 2.f) {
                    rotateLeft = true;
                }
            }

            move->m_RotationSpeed = 2 * speed * ((rotateLeft ? -1.0f : 0.0f) + (rotateRight ? 1.0f : 0.0f));
            if (bot->GetComponent<Game::BotComponent>()->m_isMoving) {
                move->m_TranslationSpeed.y = speed * (-cos(rotationRad));
                move->m_TranslationSpeed.x = speed * (sin(rotationRad));
            }
            else {
                move->m_TranslationSpeed.y = 0.0f;
                move->m_TranslationSpeed.x = 0.0f;
            }



            //posebna rotacija za turret koja prati poziciju misa
            auto turretTransf = playersTankTurret->GetComponent<Engine::TransformComponent>();
            vec2 direction = playerTransfComp->m_Position - tankTransf->m_Position;
            float angle = atan2(direction.x, direction.y) * (180.f / 3.14f);
            //malo tvikovanja i radi :D
            turretTransf->m_Rotation = angle * (-1.f) + 180.f;
        }

    }

    void Game::Bot::Reset(Engine::EntityManager* entityManager_)
    {
        auto bots = entityManager_->GetAllEntitiesWithComponent<Game::BotComponent>();

        for (auto& bot : bots) {
            auto botTank = bot->GetComponent<Game::BotComponent>()->tankEntity;
            ASSERT(botTank != nullptr, "players tank not initialized!");

            auto botTankComp = botTank->GetComponent<Game::TankComponent>();
            auto botTankTurret = botTankComp->tankTurretEntity;

            auto speed = botTank->GetComponent<Game::TankComponent>()->tankSpeed;
            auto tankTransf = botTank->GetComponent<Engine::TransformComponent>();

            //Reset speed
            botTankComp->speedBoosted = false;
            botTankComp->speedReduced = false;

            //Reset tank health
            auto healthComp = botTank->GetComponent<Engine::HealthComponent>();
            healthComp->m_CurrentHealth = healthComp->m_MaxHealth;

            //Reset tank position
            //tankTransf->m_Position = m_startingPosition;
            botTank->GetComponent<Engine::Box2dBodyComponent>()->body->SetTransform(b2Vec2(m_startingPosition.x, m_startingPosition.y), 0);

            //Reset tank rotation
            auto rotationDeg = tankTransf->m_Rotation;
            float rotationRad = (rotationDeg * 3.14f) / 180.f;
            tankTransf->m_Rotation = rotationRad;

            //Reset turret rotation
            auto turretTransf = botTankTurret->GetComponent<Engine::TransformComponent>();
            turretTransf->m_Rotation = rotationRad;
        }
    }

    float Game::Bot::distanceToPLayer(vec2 botPosition, vec2 playerPosition)
    {
        float distance = std::sqrt(std::powf(botPosition.x - playerPosition.x, 2) + std::powf(botPosition.y - playerPosition.y, 2));
        return distance;
    }


}


