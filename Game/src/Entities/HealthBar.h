#pragma once
namespace Game {

	class HealthBar {
	public:
		bool Init(Engine::EntityManager* entityManager_, Engine::Texture* texture_);
		void Update(float dt, Engine::EntityManager* entityManager_);

	};
}

