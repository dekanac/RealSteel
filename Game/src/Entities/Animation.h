#pragma once

namespace Game {

	class Animation {
	public:
		Engine::Entity* CreateAnimation(std::string name_, Engine::EntityManager* em_, Engine::TextureManager* tm_);
		void Update(float dt, Engine::EntityManager* entityManager_);
	};

}
