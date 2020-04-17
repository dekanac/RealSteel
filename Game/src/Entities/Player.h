#pragma once


namespace Engine {

	class EntityManager;
	class TextureManager;
	
}

namespace Game {
	
	class Player {
	public:
		Player() = delete;
		Player(int player_, vec2 startPos_);
		bool Init(Engine::EntityManager* entityManager_, Engine::TextureManager* textureManager_);
		void Update(float dt, Engine::EntityManager* entityManager_);
		
		void SetSpeed(float speed_);
	private:
		int m_Player;
		vec2 m_StartPos{};
	};

}

