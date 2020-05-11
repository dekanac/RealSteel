#include "precomp.h"
#include "GridSystem.h"
#include <cmath>
#include <queue>
#include <map>
#include <utility>



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

	bool GridSystem::createLake(float rotation, float screenStartingPosX, float screenStartingPosY, Engine::EntityManager* entityManager_,
		Engine::TextureManager* textureManager_, Game::StaticObject* staticObjectManager_) {

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

			staticObjectManager_->CreateLake(rotation, vec2{ static_cast<float>(x * 40 + 20), static_cast<float>(y * 40 + 20) }, entityManager_, textureManager_);
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
		Engine::EntityManager* entityManager_, Engine::TextureManager* textureManager_, Game::StaticObject* staticObjectManager_)
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

	vec2 GridSystem::BFS(std::vector<int> botCoordinates, std::vector<int> playerCoordinates)
	{
		std::map<pair, bool> visited;
		std::map<pair, pair> parents;
		std::queue<pair> Q;
		std::list<pair> path;

		for (int i = 0; i != m_sizeX; i++) {
			for (int j = 0; j != m_sizeY; j++) {
				visited[std::make_pair(i, j)] = false;
			}
		}

		pair start = std::make_pair(botCoordinates[0], botCoordinates[1]);
		pair end = std::make_pair(playerCoordinates[0], playerCoordinates[1]);

		Q.push(start);
		visited[start] = true;


		while (!Q.empty()) {
			pair current = Q.front();
			//std::cout << "Current:" << current.first << ", " << current.second << std::endl;
			Q.pop();
			visited[current] = true;

			if (current == end) {
				//Rekreiranje putanje
				while (parents[current] != start) {
					path.push_back(current);
					current = parents[current];
				}

				pair point = path.back();

				// Racunanje nove pozicije na ekranu
				vec2 newPosition = { static_cast<float>(point.first * 40 + 20), static_cast<float>(point.second * 40 + 20) };

				//// Ispis koordinata putanje
				//path.reverse();
				//for (auto i : path) {
				//	std::cout << "(" << i.first << ", " << i.second << "), ";
				//}

				//// Crtanje putanje u matricu
				//for (auto i : path) {
				//	m_TerrainGrid[i.first][i.second] = 7;
				//}
				//m_TerrainGrid[point.first][point.second] = 3;

				//// Crtanje matrice
				//std::cout << std::endl;
				//for (int i = 0; i != m_sizeX; i++) {
				//	for (int j = 0; j != m_sizeY; j++) {
				//		std::cout << m_TerrainGrid[i][j] << " ";
				//	}
				//	std::cout << "\n";
				//}
				//std::cout << std::endl;


				//// Brisanje putanje iz matrice
				//for (auto i : path) {
				//	m_TerrainGrid[i.first][i.second] = 0;
				//}

				return newPosition;
			}

			std::vector<pair> adjacents = getAdj(current);
			if (adjacents.size() != 0) {
				for (auto p : adjacents) {
					if (m_TerrainGrid[p.first][p.second] != 1) {
						if (!visited[p]) {
							visited[p] = true;
							parents[p] = current;
							Q.push(p);
						}
					}
				}
			}
		}
	}

	bool GridSystem::screenCoordinatesValid(float screenStartingPosX, float screenStartingPosY)
	{
		if (screenStartingPosX < 0 || screenStartingPosX > DEFAULT_WIDTH || screenStartingPosY < 0 || screenStartingPosY > DEFAULT_HEIGHT) {
			LOG_WARNING("Attempting to create object outside of map boundries!");
			return false;
		}
		return true;
	}
	std::vector<pair> GridSystem::getAdj(pair p)
	{
		std::vector<pair> adjacents;
		int x = p.first;
		int y = p.second;

		for (int i = x - 1; i != x + 2; i++) {
			for (int j = y - 1; j != y + 2; j++) {
				if (i < 0 || j < 0 || i > m_sizeX - 1 || j > m_sizeY - 1) {
					continue;
				}
				else if (isBlockOccupied(i, j)) {
					continue;
				}
				else {
					adjacents.push_back(std::make_pair(i, j));
				}
			}
		}

		return adjacents;
	}
}
