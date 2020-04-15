#pragma once

namespace Game {

	class AI {
	public:
		bool Init(Engine::EntityManager* entityManager_);
		void Update(float dt, Engine::EntityManager* entityManager_);

	private:
		vec2 GetPlayerPosition(Engine::EntityManager* entityManager_);
		float distance(vec2 a, vec2 b);
	};

}
