#pragma once

namespace Engine {

	class EntityManager;

	class SoundSystem {
	public:
		bool Init();
		void Update(float dt, EntityManager* entityManager_);
		bool Shutdown();

	private:

	};
}