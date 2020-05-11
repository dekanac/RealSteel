#pragma once

#include "precomp.h"

namespace Engine 
{
	class EntityManager;
	class TextureManager;
}

namespace Game 
{
	class Bot
	{
	public:
		bool AddBot(vec2 startPos_, Engine::EntityManager* entityManager_, Engine::TextureManager* textureManager_);
		void Update(float dt, Engine::EntityManager* entityManager_, Game::GridSystem* gridSystem_, Engine::SoundManager* soundManager_, Engine::TextureManager* textureManager_);
		void Reset(Engine::EntityManager* entityManager_);
	private:
		float m_time;
		vec2 m_startingPosition;
		vec2 m_newPosition;
		float distanceToPLayer(vec2 botPosition, vec2 playerPosition);
	};
}


