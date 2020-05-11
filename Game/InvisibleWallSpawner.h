#pragma once
#pragma once

namespace Game {

	class Animation;

	class InvisibleWallSpawner {
	public:
		bool Init(Engine::EntityManager* entityManager_);
		bool CreateInvisibleWall(vec2 pos_, vec2 size_, Engine::EntityManager* entityManager_);
	};
}
