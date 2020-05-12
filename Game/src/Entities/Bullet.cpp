#include "precomp.h"
#include "GameComponents.h"
#include "Bullet.h"
#include <chrono>

namespace Game {

	Engine::Entity* Bullet::CreateBullet(vec2 position_, float rotation_, bool players,
		Engine::EntityManager* entityManager_, Engine::SoundManager* soundManager_, Engine::TextureManager* textureManager_, bool special, int missilePower) {
		ASSERT(entityManager_ != nullptr, "Must pass valid pointer to entitymanager");
		ASSERT(textureManager_ != nullptr, "Must pass valid pointer to texturemanager");
		ASSERT(soundManager_ != nullptr, "Must pass valid pointer to soundmanager");

		auto bullet = std::make_unique<Engine::Entity>();
		soundManager_->PlaySound("shot", 0);

		bullet->AddComponent<Game::BulletComponent>();
		bullet->GetComponent<Game::BulletComponent>()->m_specialBullet = special;
		bullet->GetComponent<Game::BulletComponent>()->m_power = special ? 40 : missilePower;
		bullet->AddComponent<Engine::TransformComponent>(position_.x, position_.y, BULLET_SIZE_X, BULLET_SIZE_Y);
		auto transform = bullet->GetComponent<Engine::TransformComponent>();
		transform->m_Rotation = (rotation_ * 180.0f / 3.14f);
		float speedFactor = special ? 0.5f : 1.0f;
		bullet->AddComponent<Engine::MoverComponent>(cos(rotation_) * BULLET_SPEED * speedFactor, sin(rotation_) * BULLET_SPEED * speedFactor);
		bullet->AddComponent<Engine::SpriteComponent>(textureManager_->GetTexture(special ? "specialBullet" : "bullet"));
		bullet->AddComponent<Engine::CollisionComponent>(BULLET_COLLISION_X, BULLET_COLLISION_Y);

		bullet->AddComponent<OwnershipComponent>(players);

		auto flare = std::make_unique<Engine::Entity>();
		flare->AddComponent<Engine::TransformComponent>(position_.x, position_.y, FLASH_SIZE, FLASH_SIZE);
		auto flareTransform = flare->GetComponent<Engine::TransformComponent>();
		flareTransform->m_Rotation = (rotation_ * 180.0f / 3.14f);
		flare->AddComponent<Engine::SpriteComponent>(textureManager_->GetTexture("muzzleFlash"));
		flare->AddComponent<TimedComponent>(100);

		Engine::Entity* bull = bullet.get();
		entityManager_->AddEntity(std::move(bullet));
		entityManager_->AddEntity(std::move(flare));
		


		return bull;
	}

	void Bullet::Update(float dt, Engine::EntityManager* entityManager_, Engine::SoundManager* soundManager_, Engine::TextureManager* textureManager_) {
		auto bullets = entityManager_->GetAllEntitiesWithComponent<Game::BulletComponent>();
		
		for (auto& bullet : bullets) {
			
			//COLLISION
			auto bulletCollision = bullet->GetComponent<Engine::CollisionComponent>();
			if (bullet->GetComponent<BulletComponent>()->collided == true)
			{
				ExplodeBullet(bullet, entityManager_, textureManager_);
				continue;
			}
			for (auto& collided : bulletCollision->m_CollidedWith) {

				if (collided->HasComponent<Game::WallComponent>()) {
					soundManager_->PlaySound("explosion", 0);
					ExplodeBullet(bullet, entityManager_, textureManager_);
					break;
				}
			}

		}


		using namespace std::chrono;

		auto timeds = entityManager_->GetAllEntitiesWithComponent<Game::TimedComponent>();
		for (auto& timed : timeds) {
			auto timedComp = timed->GetComponent<TimedComponent>();
			duration<double, std::milli> diff = (high_resolution_clock::now() - timedComp->created);
			if (diff.count() > timedComp->lifeMillis)
				entityManager_->RemoveEntity(timed);
		}
	}
	void Bullet::ExplodeBullet(Engine::Entity* bullet, Engine::EntityManager* entityManager_, Engine::TextureManager* textureManager_)
	{
		auto transComp = bullet->GetComponent<Engine::TransformComponent>();

		auto position_ = transComp->m_Position;
		auto rotation_ = transComp->m_Rotation;
		auto flare = std::make_unique<Engine::Entity>();
		flare->AddComponent<Engine::TransformComponent>(position_.x, position_.y, FLASH_SIZE / 2, FLASH_SIZE / 2);
		flare->AddComponent<Engine::AnimationComponent>();
		auto BOOM = flare->GetComponent<Engine::AnimationComponent>();
		BOOM->m_TextureSheet = textureManager_->GetAnimationTexture("explosionAnimation");
		BOOM->m_CurrentRectToDraw = 1;
		flare->AddComponent<TimedComponent>(340);

		entityManager_->AddEntity(std::move(flare));
		entityManager_->RemoveEntity(bullet);
	}
}