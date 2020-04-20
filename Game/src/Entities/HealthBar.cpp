#include "precomp.h"
#include "HealthBar.h"
#include "GameComponents.h"

namespace Game {

	bool HealthBar::Init(Engine::EntityManager* entityManager_, Engine::TextureManager* textureManager_) {

		auto entsWithHealth = entityManager_->GetAllEntitiesWithComponent<Engine::HealthComponent>();

		for (auto& entity : entsWithHealth) {

			auto healthBar = std::make_unique<Engine::Entity>();
			healthBar->AddComponent<Engine::TransformComponent>();
			healthBar->AddComponent<Engine::SpriteComponent>();
			healthBar->AddComponent<Game::HealthBarComponent>();
			
			auto healthBarTransf = healthBar->GetComponent<Engine::TransformComponent>();
			//ako se menja, promeniti i u updateu
			healthBarTransf->m_Size[0] = HBAR_SIZE_X;
			healthBarTransf->m_Size[1] = 8.f;
			healthBar->GetComponent<Engine::SpriteComponent>()->m_Image = textureManager_->GetTexture("healthBarRed");

			auto healthBarFrame = std::make_unique<Engine::Entity>();
			healthBarFrame->AddComponent<Engine::TransformComponent>();
			healthBarFrame->AddComponent<Engine::SpriteComponent>().m_Image = textureManager_->GetTexture("healthBarFrame");
			healthBarFrame->AddComponent<Game::HealthBarFrameComponent>();
			auto healthBarFrameTransf = healthBarFrame->GetComponent<Engine::TransformComponent>();

			healthBarFrameTransf->m_Size[0] = HBAR_SIZE_X + 10.f; //frame je siri za 10.f 
			healthBarFrameTransf->m_Size[1] = 15.f;

			healthBar->GetComponent<Game::HealthBarComponent>()->healthBarFrame = healthBarFrame.get();

			entityManager_->AddEntity(std::move(healthBar));
			entityManager_->AddEntity(std::move(healthBarFrame));

		}

		return true;
	}
	void HealthBar::Update(float dt, Engine::EntityManager* entityManager_) {

		auto healthBarEntities = entityManager_->GetAllEntitiesWithComponent<Game::HealthBarComponent>();
		auto entsWithHealth = entityManager_->GetAllEntitiesWithComponent<Engine::HealthComponent>();

		for (int i = 0; i < healthBarEntities.size(); i++) {

			auto healthBarTransf = healthBarEntities[i]->GetComponent<Engine::TransformComponent>();
			auto entWithHealthTransf = entsWithHealth[i]->GetComponent<Engine::TransformComponent>();
			auto healthBarFrameTransf = healthBarEntities[i]->GetComponent<Game::HealthBarComponent>()->healthBarFrame->GetComponent<Engine::TransformComponent>();

			auto healthInfo = entsWithHealth[i]->GetComponent<Engine::HealthComponent>();
			//hardkodovana pozicija, HBAR_SIZE_X je sirina HBara, posto je Hbar centriran u odnosu na igraca
			//pozicija X se pomera levo za pola od trenutnog helta da bi bar bio levo poravnat sa igracem
			healthBarTransf->m_Size.x = HBAR_SIZE_X * (static_cast<float>(healthInfo->m_CurrentHealth) / static_cast<float>(healthInfo->m_MaxHealth));
			healthBarTransf->m_Position.x = entWithHealthTransf->m_Position.x - (HBAR_SIZE_X - healthBarTransf->m_Size.x) / 2.f;
			healthBarTransf->m_Position.y = entWithHealthTransf->m_Position.y - 50.f;
			healthBarFrameTransf->m_Position.x = entWithHealthTransf->m_Position.x;
			healthBarFrameTransf->m_Position.y = entWithHealthTransf->m_Position.y - 50.f;

		}

	}

}