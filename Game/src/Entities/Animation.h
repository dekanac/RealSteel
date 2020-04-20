#pragma once

namespace Game {

	class Animation {
	public:
		Engine::Entity* CreateAnimation(vec2 size_, unsigned int numofParts_, Engine::EntityManager* em_, Engine::Texture* texture_);
		void Update(float dt, Engine::EntityManager* entityManager_);

	};

}
