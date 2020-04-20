#pragma once

#define WALL_SIZE_X 169.f
#define WALL_SIZE_Y 46.f

namespace Game {

	class StaticObject {
	public:
		bool DrawWorld(Engine::EntityManager* em_, Engine::TextureManager* tm_);
		bool CreateWall(std::string type_, vec2 pos_, Engine::EntityManager* entityManager_, Engine::TextureManager* textureManager_);
		bool CreatePalm(vec2 pos_, Engine::EntityManager* entityManager_, Engine::TextureManager* textureManager_);
	};
}
