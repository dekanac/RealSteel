#include "precomp.h"
#include "GameStateController.h"
#include "Input/InputManager.h"
#include "ECS/Entity.h"


namespace Engine {

	bool GameStateController::Init(Engine::EntityManager* entityManager_) {
		LOG_INFO("Initializing GameStateManager");
		ASSERT(entityManager_ != nullptr, "Must pass valid pointer to entitymanager to GameStateController::Init()");

		auto gameStateController = std::make_unique<Entity>();
		entityManager_->AddEntity(std::move(gameStateController));

		return true;
	}

	void GameStateController::TogglePause() {
		m_IsPaused = !m_IsPaused;
	}

	void GameStateController::Update(float dt, EntityManager* entityManager) {
		int  i = 1;
		//bool pauseInput = InputManager::IsActionActive(1, "TogglePause"));
	}
}
