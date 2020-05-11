#include "precomp.h"
#include "GameComponents.h"
#include "Bullet.h"

namespace Game {

	Engine::Entity* Bullet::CreateBullet(vec2 position_, float rotation_, bool players,
		Engine::EntityManager* entityManager_, Engine::SoundManager* soundManager_, Engine::TextureManager* textureManager_) {
		ASSERT(entityManager_ != nullptr, "Must pass valid pointer to entitymanager");
		ASSERT(textureManager_ != nullptr, "Must pass valid pointer to texturemanager");
		ASSERT(soundManager_ != nullptr, "Must pass valid pointer to soundmanager");

		auto bullet = std::make_unique<Engine::Entity>();
		soundManager_->PlaySound("tankFire", 0);

		bullet->AddComponent<Game::BulletComponent>();
		bullet->AddComponent<Engine::TransformComponent>(position_.x, position_.y, BULLET_SIZE_X, BULLET_SIZE_Y);
		auto transform = bullet->GetComponent<Engine::TransformComponent>();
		transform->m_Rotation = (rotation_ * 180.0f / 3.14f);
		bullet->AddComponent<Engine::MoverComponent>(cos(rotation_) * BULLET_SPEED, sin(rotation_) * BULLET_SPEED);
		bullet->AddComponent<Engine::SpriteComponent>(textureManager_->GetTexture("bullet"));
		bullet->AddComponent<Engine::CollisionComponent>(BULLET_COLLISION_X, BULLET_COLLISION_Y);

		bullet->AddComponent<OwnershipComponent>(players);

		Engine::Entity* bull = bullet.get();
		entityManager_->AddEntity(std::move(bullet));

		return bull;
	}

	void Bullet::Update(float dt, Engine::EntityManager* entityManager_) {
		auto bullets = entityManager_->GetAllEntitiesWithComponent<Game::BulletComponent>();
		
		for (auto& bullet : bullets) {
			
			//COLLISION
			auto bulletCollision = bullet->GetComponent<Engine::CollisionComponent>();
			if (bullet->GetComponent<BulletComponent>()->collided == true)
			{
				entityManager_->RemoveEntity(bullet);
				continue;
			}
			for (auto& collided : bulletCollision->m_CollidedWith) {

				if (collided->HasComponent<Game::WallComponent>()) {
					entityManager_->RemoveEntity(bullet);
				}
			}

		}
	}
}