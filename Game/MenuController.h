#pragma once

#include "Core/Application.h"


namespace Game {


	class MenuController {
	public:
		bool Init(Engine::EntityManager* entityManager_, Engine::TextureManager* textureManager_);
		Engine::gameState Update(float dt, Engine::EntityManager* entityManager_, Engine::SoundManager* sm_);
	private:
		vec2 m_offScreen = { -5000.f, 0.f };
		vec2 m_onScreen = { 1280.f / 2, 720.f / 2 };

		std::vector<vec2> m_selectionPos{ vec2{ 639.5f, 351.5f }, vec2{ 639.5f, 457.5f }, vec2{ 639.5f, 563.5f } };
		int m_selectionIndex = 0;
	};
}


