#include "precomp.h"
#include "GridSystem.h"
#include <cmath>



namespace Game {

	bool GridSystem::Init()
	{
		for (int i = 0; i != m_sizeX; i++) {
			for (int j = 0; j != m_sizeY; j++) {
				m_TerrainGrid[i][j] = 0;
			}
		}
		return true;
	}

	bool GridSystem::isBlockOccupied(int x, int y)
	{
		if (m_TerrainGrid[x][y] == 0) {
			return false;
		}

		return true;
	}

	bool GridSystem::createPalmTree(float screenStartingPosX, float screenStartingPosY, Engine::EntityManager* entityManager_, 
		Engine::TextureManager* textureManager_, Game::StaticObject* staticObjectManager_, Game::Animation* animationManager_)
	{
		if (screenCoordinatesValid(screenStartingPosX, screenStartingPosY) == false) {
			return false;
		}
		else
		{
			std::vector<int> coordinates = transformToGridCoordinates(screenStartingPosX, screenStartingPosY);
			int x = coordinates[0];
			int y = coordinates[1];

			if (x > m_sizeX || y > m_sizeY) {
				LOG_WARNING("Attempting to create object outside of map boundries!");
				return false;
			}

			m_TerrainGrid[x][y] = 1; // 1 -> obstacle
			staticObjectManager_->CreatePalm(vec2{ static_cast<float>(x * 40 + 20), static_cast<float>(y * 40 + 20) }, entityManager_, textureManager_, animationManager_);
			}
			return true;
		}



	bool GridSystem::createWallHorizontal(float  screenStartingPosX, float screenStartingPosY, int amount,
		Engine::EntityManager* entityManager_, Engine::TextureManager* textureManager_, Game::StaticObject* staticObjectManager_)
	{
		if (screenCoordinatesValid(screenStartingPosX, screenStartingPosY) == false) {
			return false;
		}
		else {
			std::vector<int> coordinates = transformToGridCoordinates(screenStartingPosX, screenStartingPosY);
			int x = coordinates[0];
			int y = coordinates[1];
			int x_end = x + amount;

			if (x > m_sizeX || x_end > m_sizeX || y > m_sizeY) {
				LOG_WARNING("Attempting to create object outside of map boundries!");
				return false;
			}

			for (int i = x; i != x_end; i++) {
				if (isBlockOccupied(i, y) == true) {
					continue;
				}
				m_TerrainGrid[i][y] = 1; // 1 -> obstacle
				staticObjectManager_->CreateWallBlock("EW", vec2{ static_cast<float>(i * 40 + 20), static_cast<float>(y * 40 + 20) },
					entityManager_, textureManager_);
			}
			return true;
		}
	}


	bool GridSystem::createWallVertical(float screenStartingPosX, float screenStartingPosY, int amount,
		Engine::EntityManager * entityManager_, Engine::TextureManager * textureManager_, Game::StaticObject * staticObjectManager_)
	{
		if (screenCoordinatesValid(screenStartingPosX, screenStartingPosY) == false) {
			return false;
		}
		else
		{
			std::vector<int> coordinates = transformToGridCoordinates(screenStartingPosX, screenStartingPosY);
			int x = coordinates[0];
			int y = coordinates[1];
			int y_end = y + amount;

			if (x > m_sizeX || y > m_sizeY || y_end > m_sizeY) {
				LOG_WARNING("Attempting to create object outside of map boundries!");
				return false;
			}

			for (int i = y; i != y_end; i++) {
				if (isBlockOccupied(x, i) == true) {
					continue;
				}
				m_TerrainGrid[x][i] = 1; // 1 -> obstacle
				staticObjectManager_->CreateWallBlock("NS", vec2{ static_cast<float>(x * 40 + 20), static_cast<float>(i * 40 + 20) },
					entityManager_, textureManager_);
			}
			return true;
		}
	}

	std::vector<int> GridSystem::transformToGridCoordinates(float x_, float y_)
	{
		int x = static_cast<int>(std::ceil(x_ / 40)) - 1;
		int y = static_cast<int>(std::ceil(y_ / 40)) - 1;
		std::vector<int> coordinates{ x, y };
		return coordinates;
	}

	bool GridSystem::screenCoordinatesValid(float screenStartingPosX, float screenStartingPosY)
	{
		if (screenStartingPosX < 0 || screenStartingPosX > DEFAULT_WIDTH || screenStartingPosY < 0 || screenStartingPosY > DEFAULT_HEIGHT) {
			LOG_WARNING("Attempting to create object outside of map boundries!");
			return false;
		}
		return true;
	}
}
