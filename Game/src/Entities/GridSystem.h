#pragma once

#include "Render/WindowData.h"
#include "Entities/StaticObject.h"
#include <vector>


namespace Game {

	typedef std::pair<int, int> pair;

	class GridSystem {
	public:
		bool Init();
		bool isBlockOccupied(int x, int y);
		bool createWallHorizontal(float screenStartingPosX, float screenStartingPosY, int amount, Engine::EntityManager* entityManager_, Engine::TextureManager* textureManager_, Game::StaticObject* staticObjectManager_);
		bool createWallVertical(float screenStartingPosX, float screenStartingPosY, int amount, Engine::EntityManager* entityManager_, Engine::TextureManager* textureManager_, Game::StaticObject* staticObjectManager_);
		bool createPalmTree(float screenStartingPosX, float screenStartingPosY, Engine::EntityManager* entityManager_, Engine::TextureManager* textureManager_, Game::StaticObject* staticObjectManager_, Game::Animation* animationManager_);
		bool createLake(float rotation, float screenStartingPosX, float screenStartingPosY, Engine::EntityManager* entityManager_, Engine::TextureManager* textureManager_, Game::StaticObject* staticObjectManager_);
		std::vector<int> transformToGridCoordinates(float x, float y);
		vec2 BFS(std::vector<int> botCoordinates, std::vector<int> playerCoordinates);

	private:
		int m_sizeX = static_cast<int>(DEFAULT_WIDTH / 40);
		int m_sizeY = static_cast<int>(DEFAULT_HEIGHT / 40);
		std::vector< std::vector<int> > m_TerrainGrid{ m_sizeX , std::vector<int>(m_sizeY) };
		bool screenCoordinatesValid(float screenStartingPosX, float screenStartingPosY);
		std::vector<pair> getAdj(pair p);
	};
}

