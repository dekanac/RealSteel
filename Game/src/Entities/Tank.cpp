#include "precomp.h"
#include "GameComponents.h"
#include "Tank.h"
#include <Box2d.h>

namespace Game {

	Engine::Entity* Tank::CreateTank(vec2 position_, Engine::EntityManager* entityManager_, Engine::TextureManager* textureManager_) {
		
		ASSERT(entityManager_ != nullptr, "Must pass valid pointer to entitymanager");
		ASSERT(textureManager_ != nullptr, "Must pass valid pointer to texturemanager");

		auto tank = std::make_unique<Engine::Entity>();

		tank->AddComponent<Engine::Box2dBodyComponent>();

		b2BodyDef bodyDef;
		bodyDef.type = b2_dynamicBody;
		bodyDef.bullet = true;
		bodyDef.linearDamping = 0.10f;
		bodyDef.position.Set(position_.x, position_.y);
		
		auto physicsComponent = entityManager_->GetAllComponentInstances<Game::PhysicsComponent>().back();
		tank->GetComponent<Engine::Box2dBodyComponent>()->body = physicsComponent->world->CreateBody(&bodyDef);
		
		// Define another box shape for our dynamic body.
		b2PolygonShape polygonShape;
		polygonShape.SetAsBox(30, 30);
		// Define the dynamic body fixture.
		b2FixtureDef fixtureDef;
		fixtureDef.shape = &polygonShape;
		// Set the box density to be non-zero, so it will be dynamic.
		fixtureDef.density = 1.0f;
		// Override the default friction.
		fixtureDef.friction = 0.02f;
		// Add the shape to the body.
		tank->GetComponent<Engine::Box2dBodyComponent>()->body->CreateFixture(&fixtureDef);

		//transformation and move
		tank->AddComponent<Engine::TransformComponent>(position_.x, position_.y, TANK_SIZE_X, TANK_SIZE_Y);
		tank->AddComponent<Engine::MoverComponent>();
		tank->AddComponent<Engine::CollisionComponent>(TANK_COLISION_X, TANK_COLISION_Y);
		tank->AddComponent<Game::TankComponent>();
		tank->AddComponent<Game::SolidObjectComponent>();

		//health
		tank->AddComponent<Engine::HealthComponent>();
		auto health = tank->GetComponent<Engine::HealthComponent>();
		health->m_MaxHealth = 100;
		health->m_CurrentHealth = 40;
		//TANK BODY INIT
		auto tankBody = std::make_unique<Engine::Entity>();
		tankBody->AddComponent<Engine::TransformComponent>(position_.x, position_.y, TANK_SIZE_X, TANK_SIZE_Y);
		//samo zbog crtanja kvadrata u debagu, jer glavni tank component nema sprite komponentu
		tankBody->AddComponent<Engine::CollisionComponent>(TANK_COLISION_X, TANK_COLISION_Y);
		tankBody->AddComponent<Engine::SpriteComponent>(textureManager_->GetTexture("tankBody"));
		tankBody->AddComponent<Engine::ShadowComponent>();

	
		//TANK TURRET INIT
		auto tankTurret = std::make_unique<Engine::Entity>();
		tankTurret->AddComponent<Engine::TransformComponent>(position_.x, position_.y, TANK_SIZE_X, TANK_SIZE_Y);
		tankTurret->AddComponent<Engine::MoverComponent>();
		tankTurret->AddComponent<Engine::SpriteComponent>(textureManager_->GetTexture("tankTurret"));
		tankTurret->AddComponent<Engine::ShadowComponent>();

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

			//body i turret update
			tankBodyTransformComp->m_Position = tankTransformComp->m_Position;
			tankBodyTransformComp->m_Rotation = tankTransformComp->m_Rotation;
			tankTurretTransformComp->m_Position = tankTransformComp->m_Position;

			//COLLISION
			auto tankCollision = tank->GetComponent<Engine::CollisionComponent>();
			tankComp->speedReduced = false;

			for (auto& collided : tankCollision->m_CollidedWith) {

				if (collided->HasComponent<Game::LakeComponent>() || tankComp->shieldActivated) {
					tankComp->speedReduced = true;
				}
			}

			if (tankComp->speedBoosted && tankComp->speedReduced) {
				tankComp->tankSpeed = TANK_SPEED * 0.75; 
			}
			else if (tankComp->speedBoosted) {
				tankComp->tankSpeed = TANK_SPEED * 100.5; 
			}
			else if (tankComp->speedReduced) { 
				tankComp->tankSpeed = TANK_SPEED * 0.5; 
			}
			else { 
				tankComp->tankSpeed = TANK_SPEED;  
			}
		}
	}
}
