#include "precomp.h"
#include "HealthBar.h"
#include "GameComponents.h"

namespace Game {

	bool HealthBar::Init(Engine::EntityManager* entityManager_, Engine::Texture* texture_) {

		auto entsWithHealth = entityManager_->GetAllEntitiesWithComponent < Engine::HealthComponent>();

		for (auto& entity : entsWithHealth) {

			auto healthBar = std::make_unique<Engine::Entity>();
			healthBar->AddComponent<Engine::TransformComponent>();
			healthBar->AddComponent<Engine::SpriteComponent>();
			healthBar->AddComponent<Game::HealthBarComponent>();
			auto healthBarSize = healthBar->GetComponent<Engine::TransformComponent>();
			//ako se menja, promeniti i u updateu
			healthBarSize->m_Size[0] = 50.f;
			healthBarSize->m_Size[1] = 8.f;

			healthBar->GetComponent<Engine::SpriteComponent>()->m_Image = texture_;

			entityManager_->AddEntity(std::move(healthBar));

		}


		return true;
	}
	void HealthBar::Update(float dt, Engine::EntityManager* entityManager_) {

		auto healthBarEntities = entityManager_->GetAllEntitiesWithComponent<Game::HealthBarComponent>();
		auto entsWithHealth = entityManager_->GetAllEntitiesWithComponent<Engine::HealthComponent>();

		for (int i = 0; i < healthBarEntities.size(); i++) {

			auto healthBarTransf = healthBarEntities[i]->GetComponent<Engine::TransformComponent>();
			auto entWithHealthPos = entsWithHealth[i]->GetComponent<Engine::TransformComponent>();

			auto healthInfo = entsWithHealth[i]->GetComponent<Engine::HealthComponent>();

			//hardkodovana pozicija, 50.f je sirina HBara, posto je Hbar centriran u odnosu na igraca
			//pozicija X se pomera levo za pola od trenutnog helta da bi bar bio levo poravnat sa igracem
			healthBarTransf->m_Size[0] = 50.f *(static_cast<float>(healthInfo->m_CurrentHealth) / static_cast<float>(healthInfo->m_MaxHealth));
			healthBarTransf->m_Position[0] = entWithHealthPos->m_Position[0] - (50.f - healthBarTransf->m_Size[0]) / 2.f;
			healthBarTransf->m_Position[1] = entWithHealthPos->m_Position[1] - 30.f;
			
		}

	}
}