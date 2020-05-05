#include "precomp.h"
#include "MenuController.h"

bool Game::MenuController::Init(Engine::EntityManager* entityManager_, Engine::TextureManager* textureManager_)
{
	ASSERT(textureManager_ != nullptr, "nullptr passed as textureManager_");
	ASSERT(entityManager_ != nullptr, "nullptr passed as entityManager_");

	// Menu Sceen
	auto mainMenu = std::make_unique<Engine::Entity>();

	mainMenu->AddComponent<Engine::TransformComponent>(m_offScreen);
	mainMenu->AddComponent<Engine::SpriteComponent>();
	mainMenu->AddComponent<Engine::MenuComponent>();
	mainMenu->AddComponent<Engine::InputComponent>();
	
	mainMenu->GetComponent<Engine::InputComponent>()->inputActions.push_back({"Menu", Engine::EInputActionState::JustPressed });
	mainMenu->GetComponent<Engine::SpriteComponent>()->m_Image = textureManager_->GetTexture("mainMenu");
	mainMenu->GetComponent<Engine::MenuComponent>()->m_state = true;

	entityManager_->AddEntity(std::move(mainMenu));

	// Pause Sceen
	auto pauseMenu = std::make_unique<Engine::Entity>();

	pauseMenu->AddComponent<Engine::TransformComponent>(m_offScreen);
	pauseMenu->AddComponent<Engine::SpriteComponent>();
	pauseMenu->AddComponent<Engine::MenuComponent>();
	pauseMenu->AddComponent<Engine::InputComponent>();

	pauseMenu->GetComponent<Engine::InputComponent>()->inputActions.push_back({ "Menu", Engine::EInputActionState::JustPressed });
	pauseMenu->GetComponent<Engine::SpriteComponent>()->m_Image = textureManager_->GetTexture("pauseMenu");

	entityManager_->AddEntity(std::move(pauseMenu));

	// Selection Box
	auto selectionBox = std::make_unique<Engine::Entity>();

	selectionBox->AddComponent<Engine::TransformComponent>(m_offScreen);
	selectionBox->AddComponent<Engine::SpriteComponent>();
	selectionBox->AddComponent<Engine::MenuComponent>();
	selectionBox->AddComponent<Engine::InputComponent>();

	selectionBox->GetComponent<Engine::InputComponent>()->inputActions.push_back({ "Enter", Engine::EInputActionState::JustPressed });
	selectionBox->GetComponent<Engine::InputComponent>()->inputActions.push_back({ "Player1MoveUp", Engine::EInputActionState::JustPressed });
	selectionBox->GetComponent<Engine::InputComponent>()->inputActions.push_back({ "Player1MoveDown", Engine::EInputActionState::JustPressed });
	selectionBox->GetComponent<Engine::SpriteComponent>()->m_Image = textureManager_->GetTexture("selectionBox");

	entityManager_->AddEntity(std::move(selectionBox));

	return true;
}

Engine::gameState Game::MenuController::Update(float dt, Engine::EntityManager* entityManager_, Engine::SoundManager* sm_)
{
	auto mainMenu = entityManager_->GetAllEntitiesWithComponent<Engine::MenuComponent>()[0];
	auto pauseMenu = entityManager_->GetAllEntitiesWithComponent<Engine::MenuComponent>()[1];
	auto selectionBox = entityManager_->GetAllEntitiesWithComponent<Engine::MenuComponent>()[2];

	auto mainMenuComp = mainMenu->GetComponent<Engine::MenuComponent>();
	auto mainMenuInputComp = mainMenu->GetComponent<Engine::InputComponent>();
	auto mainMenuMoveComp = mainMenu->GetComponent<Engine::TransformComponent>();
	//bool mainMenuIsPressed = Engine::InputManager::IsActionActive(mainMenuInputComp, "Menu");

	auto pauseMenuComp = pauseMenu->GetComponent<Engine::MenuComponent>();
	auto pauseMenuInputComp = pauseMenu->GetComponent<Engine::InputComponent>();
	auto pauseMenuMoveComp = pauseMenu->GetComponent<Engine::TransformComponent>();
	bool pauseMenuIsPressed = Engine::InputManager::IsActionActive(pauseMenuInputComp, "Menu");

	auto selectionBoxInputComp = selectionBox->GetComponent<Engine::InputComponent>();
	auto selectionBoxMoveComp = selectionBox->GetComponent<Engine::TransformComponent>();


	//pauseMenuComp->m_state = pauseMenuIsPressed && !pauseMenuComp->m_state;

	if (pauseMenuIsPressed && !pauseMenuComp->m_state) {
		if (!mainMenuComp->m_state) {
			pauseMenuComp->m_state = true;
		}
	}

	else if (pauseMenuIsPressed && pauseMenuComp->m_state) {
		pauseMenuComp->m_state = false;
	}


	if (pauseMenuComp->m_state) {
		mainMenuMoveComp->m_Position = m_offScreen;
		pauseMenuMoveComp->m_Position = m_onScreen;

		bool selectionBoxUpPressed = Engine::InputManager::IsActionActive(selectionBoxInputComp, "Player1MoveUp");
		bool selectionBoxDownPressed = Engine::InputManager::IsActionActive(selectionBoxInputComp, "Player1MoveDown");
		bool selectionBoxEnterPressed = Engine::InputManager::IsActionActive(selectionBoxInputComp, "Enter");
		if (selectionBoxDownPressed) {
			m_selectionIndex++;
			sm_->PlaySound("menuScroll", 0);
		}

		if (selectionBoxUpPressed) {
			m_selectionIndex--;
			sm_->PlaySound("menuScroll", 0);
		}
		m_selectionIndex = (3 + (m_selectionIndex % 3)) % 3;
		selectionBoxMoveComp->m_Position = m_selectionPos[m_selectionIndex];

		if (selectionBoxEnterPressed) {
			sm_->PlaySound("menuConfirm", 0);
			switch (m_selectionIndex) {
			case 0:
				pauseMenuComp->m_state = false;
				return Engine::gameState::RESTART;
			case 1:
				mainMenuComp->m_state = true;
				pauseMenuComp->m_state = false;
				m_selectionIndex = 0;
				break;
			case 2:
				return Engine::gameState::STOPPED;
			}
		}
	}

	else if (mainMenuComp->m_state) {
		pauseMenuMoveComp->m_Position = m_offScreen;
		mainMenuMoveComp->m_Position = m_onScreen;
		bool selectionBoxUpPressed = Engine::InputManager::IsActionActive(selectionBoxInputComp, "Player1MoveUp");
		bool selectionBoxDownPressed = Engine::InputManager::IsActionActive(selectionBoxInputComp, "Player1MoveDown");
		bool selectionBoxEnterPressed = Engine::InputManager::IsActionActive(selectionBoxInputComp, "Enter");
		if (selectionBoxDownPressed) {
			m_selectionIndex++;
			sm_->PlaySound("menuScroll", 0);
		}

		if (selectionBoxUpPressed) {
			m_selectionIndex--;
			sm_->PlaySound("menuScroll", 0);
		}
		m_selectionIndex = (3 + (m_selectionIndex % 3)) % 3;
		selectionBoxMoveComp->m_Position = m_selectionPos[m_selectionIndex];

		if (selectionBoxEnterPressed) {
			sm_->PlaySound("menuConfirm", 0);
			switch (m_selectionIndex) {
				case 0:
					mainMenuComp->m_state = false;
					return Engine::gameState::RUNNING;
				case 1:
					//TODO
					m_selectionIndex = 0;
					break;
				case 2:
					return Engine::gameState::STOPPED;
			}
		}
	}
	else {
		pauseMenuMoveComp->m_Position = m_offScreen;
		mainMenuMoveComp->m_Position = m_offScreen;
		selectionBoxMoveComp->m_Position = m_offScreen;
	}

	return mainMenuComp->m_state ? Engine::gameState::IN_MENU : (pauseMenuComp->m_state ? Engine::gameState::PAUSED : Engine::gameState::RUNNING );
}

