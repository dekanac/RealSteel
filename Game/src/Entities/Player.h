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
		Player(int player_, vec2 startPos_, float speed_);
		bool Init(Engine::EntityManager* entityManager_, Engine::Texture* texture_);
		void Update(float dt, Engine::EntityManager* entityManager_);
		
		void SetSpeed(float speed_);
	private:
		int m_Player;
		float m_Speed{ 100.f };
		vec2 m_StartPos{};
	};


}

