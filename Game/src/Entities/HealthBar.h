#pragma once

#define HBAR_SIZE_X 70.f

namespace Game {

	class HealthBar {
	public:
		bool Init(Engine::EntityManager* entityManager_, Engine::TextureManager* textureManager_);
		void Update(float dt, Engine::EntityManager* entityManager_);
	};
}

