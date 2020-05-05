#pragma once

namespace Engine {

	class EntityManager;

	class GameStateController
	{
	public:
		bool Init(Engine::EntityManager* entityManager_);
		void TogglePause();
		void Update(float dt, EntityManager* entityManager);

	private:
		bool m_IsPaused = false;
	};
}


