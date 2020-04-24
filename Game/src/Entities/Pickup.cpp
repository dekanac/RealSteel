#include "precomp.h"
#include "Pickup.h"

#include "GameComponents.h"


#include <ctime>
#include <cstdlib>

bool pickup_exists{ false };
time_t last_pickup;

namespace Game {

	bool Pickup::Init(Engine::EntityManager* em_, Engine::TextureManager* tm_) {
		
		last_pickup = game_start_time;

		//napravi sve pickupe van mape
		auto res =	CreatePickup("speed", vec2{ -100.f, -100.f }, em_, tm_) &&
					CreatePickup("health", vec2{ -100.f, -100.f }, em_, tm_) &&
					CreatePickup("ammunition", vec2{ -100.f, -100.f }, em_, tm_);

		return res;
	}

	bool Pickup::CreatePickup(std::string type_, vec2 pos_, Engine::EntityManager* em_, Engine::TextureManager* tm_) {
	
		PickupType type;
		if (type_ == "speed") { type = PickupType::Speed; }
		else if (type_ == "health") { type = PickupType::Health; }
		else if (type_ == "ammunition") { type = PickupType::Ammunation; }
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
	void Pickup::Update(float dt, Engine::EntityManager* em_, Engine::SoundManager* sm_) {
		
		auto tanks = em_->GetAllEntitiesWithComponents<Game::TankComponent, Engine::HealthComponent>();
		auto pickups = em_->GetAllEntitiesWithComponent<Game::PickupComponent>();

		//generisi random pickup na random slobodnom mestu u svetu
		if (pickup_exists == false && (pickups.size() != 0)) {
			
			//10 sekundi posle pokupljenog pikapa generisi novi
			if ((time(0) - last_pickup) > 10) {
				
				auto randomPickup = pickups[rand() % pickups.size()];
				auto pickupTransf = randomPickup->GetComponent<Engine::TransformComponent>();
				vec2 randomPosition = vec2{ rand() % 1240 + 20, rand() % 680 + 20 };
				//provera da li je pozicija slobodna, da nije zid ili palma
				
				pickupTransf->m_Position = randomPosition;
				pickup_exists = true;
			}			
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
						
						tankComp->tankSpeed = 150.f;
					}

					if (pickupComp->m_Type == PickupType::Health) {
						
						tankHealth->m_CurrentHealth = 100;
					}
					
					if (pickupComp->m_Type == PickupType::Ammunation) {
					
						//TODO::ammunition pickup dodaje municiju
					}		

					sm_->PlaySound("pickup", 0);
					auto colidedEntTransf = collidedEnt->GetComponent<Engine::TransformComponent>();
					//ako je pokupljen pickup premesta se van mape, tj "NESTAJE"
					colidedEntTransf->m_Position = vec2{ -100.f, -100.f };
					pickup_exists = false;
					last_pickup = time(0);
				}
			}
		}
 	}
}
