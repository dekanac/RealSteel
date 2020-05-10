#pragma once


namespace Game {

#define RANDOM_SEC_RANGE(x,y)  ((rand() % x) + (y-x))
#define PICKUP_SIZE 30.f

	class Pickup {
	public:
		bool Init(Engine::EntityManager* em_, Engine::TextureManager* tm_);
		void Update(float dt, Engine::EntityManager* em_, Engine::SoundManager* sm_);
		void Reset(Engine::EntityManager* em_);
	private:
		bool m_pickupExists{ false };
		time_t m_lastPickup;
		bool CreatePickup(std::string type_, vec2 pos_, Engine::EntityManager* em_, Engine::TextureManager* tm_);
		vec2 FindPlace();
		void PutPickup(std::vector<Engine::Entity*> pickups);
	};

}