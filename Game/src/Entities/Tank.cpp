#include "precomp.h"
#include "GameComponents.h"
#include "Tank.h"

namespace Game {

	Engine::Entity* Tank::CreateTank(vec2 position_, Engine::EntityManager* entityManager_, Engine::TextureManager* textureManager_) {
		
		ASSERT(entityManager_ != nullptr, "Must pass valid pointer to entitymanager");
		ASSERT(textureManager_ != nullptr, "Must pass valid pointer to texturemanager");

		auto tank = std::make_unique<Engine::Entity>();
		
		//transformation and move
		tank->AddComponent<Engine::TransformComponent>(position_.x, position_.y, 35.f, 65.f);
		tank->AddComponent<Engine::MoverComponent>();
		tank->AddComponent<Engine::CollisionComponent>(40.f, 40.f);
		tank->AddComponent<Game::TankComponent>();

		//health
		tank->AddComponent<Engine::HealthComponent>();
		auto health = tank->GetComponent<Engine::HealthComponent>();
		health->m_MaxHealth = 100;
		health->m_CurrentHealth = health->m_MaxHealth;

		auto tankBody = std::make_unique<Engine::Entity>();
		tankBody->AddComponent<Engine::TransformComponent>(position_.x, position_.y, 35.f, 65.f);
		//samo zbog crtanja kvadrata u debagu, jer glavni tank component nema sprite komponentu
		tankBody->AddComponent<Engine::CollisionComponent>(40.f, 40.f);
		tankBody->AddComponent<Engine::SpriteComponent>(textureManager_->GetTexture("tankBody"));
		

		auto tankTurret = std::make_unique<Engine::Entity>();
		tankTurret->AddComponent<Engine::TransformComponent>(position_.x, position_.y, 35.f, 65.f);
		tankTurret->AddComponent<Engine::MoverComponent>();
		tankTurret->AddComponent<Engine::SpriteComponent>(textureManager_->GetTexture("tankTurret"));

		auto tankComponent = tank->GetComponent<Game::TankComponent>();
		tankComponent->tankBodyEntity = tankBody.get();
		tankComponent->tankTurretEntity = tankTurret.get();

		entityManager_->AddEntity(std::move(tankBody));
		entityManager_->AddEntity(std::move(tankTurret));

		auto tankPtr = tank.get();
		entityManager_->AddEntity(std::move(tank));

		return tankPtr;
	}
	void Tank::Update(float dt, Engine::EntityManager* entityManager_) {
		
		auto tanks = entityManager_->GetAllEntitiesWithComponent<Game::TankComponent>();
		
		for (auto& tank : tanks) {

			auto tankComp = tank->GetComponent<Game::TankComponent>();
			auto tankTransformComp = tank->GetComponent<Engine::TransformComponent>();
			auto tankBodyTransformComp = tankComp->tankBodyEntity->GetComponent<Engine::TransformComponent>();
			auto tankTurretTransformComp = tankComp->tankTurretEntity->GetComponent<Engine::TransformComponent>();

			
			tankBodyTransformComp->m_Position = tankTransformComp->m_Position;
			tankBodyTransformComp->m_Rotation = tankTransformComp->m_Rotation;

			tankTurretTransformComp->m_Position = tankTransformComp->m_Position;

		}

	}
}
