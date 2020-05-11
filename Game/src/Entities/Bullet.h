#pragma once

namespace Game {

	#define BULLET_SIZE_X 16.f
	#define BULLET_SIZE_Y 6.f

	#define BULLET_COLLISION_X 15.f
	#define BULLET_COLLISION_Y 15.f

	#define BULLET_SPEED 1700.0f

	class Bullet
	{
	public:
		static Engine::Entity* CreateBullet(vec2 position_, float rotation_, bool players,
			Engine::EntityManager* entityManager_, Engine::SoundManager* soundManager_, Engine::TextureManager* textureManager_, bool special, int missilePower);

		static void Update(float dt, Engine::EntityManager* entityManager_, Engine::SoundManager* soundManager_);
	};
}