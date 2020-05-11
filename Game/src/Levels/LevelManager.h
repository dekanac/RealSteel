#pragma once


#include "Core/Application.h"

namespace Game {

	class Terrain;
	class Player;
	class Tank;
	class StaticObject;
	class Pickup;
	class GridSystem;
	class Animation;

	class LevelManager {
	public:
		void Init(Engine::EntityManager* em_, Engine::TextureManager* tm_, Game::Animation* ac_);
		void Update(float dt, Engine::EntityManager* em_, Engine::SoundManager* sm_, Engine::TextureManager* tm_);
		void LoadLevel(int level, Engine::EntityManager* em_, Engine::TextureManager* tm_, Game::Animation* ac_, Game::GridSystem* gs_, Game::StaticObject* sc_);
		void ResetLevel(Engine::EntityManager* em_);
	private:
		std::unique_ptr<Terrain> m_Terrain{};
		std::unique_ptr<Player> m_PlayersController{};
		std::unique_ptr<Tank> m_TanksController{};
		std::unique_ptr<StaticObject> m_StaticObjectsController{};
		std::unique_ptr<Pickup> m_PickupsController{};
		std::unique_ptr<GridSystem> m_GridSystem{};
	};
}



