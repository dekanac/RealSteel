#pragma once

#define TANK_SIZE_X 50.f
#define TANK_SIZE_Y 90.f

#define TANK_COLISION_X 55.f
#define TANK_COLISION_Y 55.f

namespace Game {

	class Tank {
	public:
		Engine::Entity* CreateTank(vec2 position_, Engine::EntityManager* entityManager_, Engine::TextureManager* textureManager_);
		void Update(float dt, Engine::EntityManager* entityManager_);
	
	};


}
