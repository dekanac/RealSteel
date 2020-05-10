#include "precomp.h"
#include "Pickup.h"
#include "GameComponents.h"

#include <ctime>
#include <cstdlib>

namespace Game {

	bool Pickup::Init(Engine::EntityManager* em_, Engine::TextureManager* tm_) {
		
		ASSERT(tm_ != nullptr, "nullptr passed as textureManager_");
		ASSERT(em_ != nullptr, "nullptr passed as entityManager_");


		m_lastPickup = game_start_time;

		//napravi sve pickupe van mape
		auto res =	CreatePickup("speed", vec2{ -100.f, -100.f }, em_, tm_) &&
					CreatePickup("health", vec2{ -100.f, -100.f }, em_, tm_) &&
					CreatePickup("ammunition", vec2{ -100.f, -100.f }, em_, tm_) &&
					CreatePickup("max_ammunition", vec2{-100.f, -100.f}, em_, tm_) &&
					CreatePickup("health_help", vec2{ -100.f, -100.f }, em_, tm_) &&
					CreatePickup("missile_powerup", vec2{ -100.f, -100.f }, em_, tm_) &&
					CreatePickup("special_missile", vec2{ -100.f, -100.f }, em_, tm_) &&
					CreatePickup("shield", vec2{ -100.f, -100.f}, em_, tm_);

		return res;
	}

	bool Pickup::CreatePickup(std::string type_, vec2 pos_, Engine::EntityManager* em_, Engine::TextureManager* tm_) {
	
		PickupType type;
		if (type_ == "speed") { type = PickupType::Speed; }
		else if (type_ == "health") { type = PickupType::Health; }
		else if (type_ == "health_help") { type = PickupType::Health; }
		else if (type_ == "ammunition") { type = PickupType::Ammunation; }
		else if (type_ == "max_ammunition") { type = PickupType::Max_Ammunition; }
		else if (type_ == "missile_powerup") { type = PickupType::Missile_Powerup; }
		else if (type_ == "special_missile") { type = PickupType::Special_Missile; }
		else if(type_ == "shield") { type = PickupType::Shield; }
		else { LOG_ERROR("type:{}, not defined!", type_); }

		auto entity = std::make_unique<Engine::Entity>();
		entity->AddComponent<Engine::SpriteComponent>(tm_->GetTexture({ fmt::format("{}Pickup", type_)}));
		entity->AddComponent<Game::PickupComponent>(type);
		entity->AddComponent<Engine::TransformComponent>(pos_, vec2{ PICKUP_SIZE, PICKUP_SIZE });
		entity->AddComponent<Engine::CollisionComponent>(PICKUP_SIZE, PICKUP_SIZE);
		entity->AddComponent<Engine::ShadowComponent>();

		em_->AddEntity(std::move(entity));

		return true;
	}

	vec2 Pickup::FindPlace() {
		vec2 position;
		do {
			position = vec2{ rand() % 1240 + 20, rand() % 680 + 20 };
		} while (false /*TODO: uslov zaustavljanja, da nema objekata na toj poziciji*/);
		
		return position;
	}

	void Pickup::PutPickup(std::vector<Engine::Entity*> pickups) {
		
	
			//X sekundi posle pokupljenog pikapa generisi novi
			if ((time(0) - m_lastPickup) > 30) {

				auto randomPickup = pickups[rand() % pickups.size()];

				auto type = randomPickup->GetComponent<Game::PickupComponent>()->m_Type;


				auto prob = static_cast<double>(rand()) / RAND_MAX;

				std::cout << prob;

				if (type == PickupType::Health && prob > 0.5) {
					return;
				}
				else if (type == PickupType::Max_Ammunition && prob > 0.4) {
					return;
				}
				else if (type == PickupType::Missile_Powerup && prob > 0.5) {
					return;
				}
				else if (type == PickupType::Special_Missile && prob > 0.3) {
					return;
				}
				else if (type == PickupType::Shield && prob && prob > 0.7) {
					return;
				}

				auto pickupTransf = randomPickup->GetComponent<Engine::TransformComponent>();
				auto randomPosition = FindPlace();
				//TODO: provera da li je pozicija slobodna, da nije zid ili palma

				pickupTransf->m_Position = randomPosition;
				m_pickupExists = true;
			}
		
	}

	void Pickup::Update(float dt, Engine::EntityManager* em_, Engine::SoundManager* sm_) {
		
		auto tanks = em_->GetAllEntitiesWithComponents<Game::TankComponent, Engine::HealthComponent>();
		auto pickups = em_->GetAllEntitiesWithComponent<Game::PickupComponent>();

		//generisi random pickup na random slobodnom mestu u svetu
		if (m_pickupExists == false && (pickups.size() != 0)) {
			PutPickup(pickups);
		}
		
		//proveri da li je neko pokupio pickup i update-uj odgovarajuci entity
		for (auto& tank : tanks) {

			auto tankCollision = tank->GetComponent<Engine::CollisionComponent>();
			auto tankComp = tank->GetComponent<Game::TankComponent>();
			auto tankHealth = tank->GetComponent<Engine::HealthComponent>();

			for (auto& collidedEnt : tankCollision->m_CollidedWith) {

				if (collidedEnt->HasComponent<Game::PickupComponent>()) {

					auto pickupComp = collidedEnt->GetComponent<Game::PickupComponent>();

					if (pickupComp->m_Type == PickupType::Speed) {

						tankComp->speedBoosted = true;
					}

					if (pickupComp->m_Type == PickupType::Health) {

						tankHealth->m_CurrentHealth = 100;
					}

					if (pickupComp->m_Type == PickupType::Health_Help) {
						tankHealth->m_CurrentHealth = tankHealth->m_CurrentHealth + 20 < 100 ?
							tankHealth->m_CurrentHealth + 20 : 100;
					}

					if (pickupComp->m_Type == PickupType::Shield) {
						tankComp->shieldActivated = true;
						tankComp->speedReduced = true;
					}

					if (pickupComp->m_Type == PickupType::Ammunation) {

						//TODO::ammunition pickup dodaje municiju
					}

					if (pickupComp->m_Type == PickupType::Max_Ammunition) {
						//TODO::max_ammunition povecava municiju na max dozvoljenog
					}

					if (pickupComp->m_Type == PickupType::Missile_Powerup) {

						//TODO::Missile_powerup pojacava snagu raketa, 
						//na odredjeno vreme ili do kraja igre, kako se dogovorimo
					}

					if (pickupComp->m_Type == PickupType::Special_Missile) {

						//TODO::Special_missile dodaje specijalnu raketu 
						//(koja moze da se gleda kao specijalan napad)
					}

					sm_->PlaySound("pickup", 0);
					auto colidedEntTransf = collidedEnt->GetComponent<Engine::TransformComponent>();
					//ako je pokupljen pickup premesta se van mape, tj "NESTAJE"
					colidedEntTransf->m_Position = vec2{ -100.f, -100.f };
					m_pickupExists = false;
					m_lastPickup = time(0);
				}
			}
		

			//vreme trajanja speedup-a
			if (tankComp->speedBoosted == true && time(0) - m_lastPickup > 20) {
				tankComp->speedBoosted = false;
			}
		}
 	}
	void Pickup::Reset(Engine::EntityManager* em_)
	{
		auto pickups = em_->GetAllEntitiesWithComponent<Game::PickupComponent>();
		for (auto pickup : pickups) {
			pickup->GetComponent<Engine::TransformComponent>()->m_Position = vec2{ -100.f, -100.f };
		}

		if (pickups.size() != 0) {
			auto randomPickup = pickups[rand() % pickups.size()];
			auto pickupTransf = randomPickup->GetComponent<Engine::TransformComponent>();
			vec2 randomPosition = vec2{ rand() % 1240 + 20, rand() % 680 + 20 };
			pickupTransf->m_Position = randomPosition;
			m_pickupExists = true;
		}
	}
}
