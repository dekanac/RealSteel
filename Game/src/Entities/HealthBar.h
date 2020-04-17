#pragma once
namespace Game {

	class HealthBar {
	public:
		bool Init(Engine::EntityManager* entityManager_, Engine::TextureManager* textureManager_);
		void Update(float dt, Engine::EntityManager* entityManager_);

	};
}

