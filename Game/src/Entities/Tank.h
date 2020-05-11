#pragma once
#define __STDC_LINMIT_MACROS 1
#include <Box2d.h>
#include <chrono>

namespace Game {


#define TANK_SIZE_X 50.f
#define TANK_SIZE_Y 90.f

#define TANK_COLISION_X 55.f
#define TANK_COLISION_Y 55.f

#define TANK_SPEED 100.f

	class Tank {
	public:
		Engine::Entity* CreateTank(vec2 position_, Engine::EntityManager* entityManager_, Engine::TextureManager* textureManager_);
		
		b2Body* body;
		
		void Update(float dt, Engine::EntityManager* entityManager_, Engine::TextureManager* textureManager_);

		static void Shoot(Engine::Entity* e, bool players, Engine::EntityManager* entityManager_, Engine::SoundManager* soundManager_, Engine::TextureManager* textureManager_);
	
	};


}

