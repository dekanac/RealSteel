#pragma once

namespace Game {

	class Tank {
	public:
		Engine::Entity* CreateTank(vec2 position_, Engine::EntityManager* entityManager_, Engine::TextureManager* textureManager_);
		void Update(float dt, Engine::EntityManager* entityManager_);
	
	};


}
