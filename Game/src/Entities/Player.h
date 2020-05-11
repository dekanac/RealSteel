#pragma once


namespace Engine {

	class EntityManager;
	class TextureManager;
	
}

namespace Game {
	
	class Player {
	public:
		bool AddPlayer(vec2 startPos_, Engine::EntityManager* entityManager_, Engine::TextureManager* textureManager_);
		void Update(float dt, Engine::EntityManager* entityManager_, Engine::SoundManager* soundManager_, Engine::TextureManager* textureManager_);
		void Reset(Engine::EntityManager* entityManager_);
		
	private:
		vec2 m_startingPosition;
		inline static unsigned int m_Player = 0;
	};

}

