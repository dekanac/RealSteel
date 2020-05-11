#include "precomp.h"
#include "InputManager.h"

#include "ECS/EntityManager.h"
#include <SDL.h>

namespace Engine
{
    bool KeyDown(KeyboardButton _iKey)
    {
        short iState = GetAsyncKeyState(static_cast<int>(_iKey));
        return (iState & 0x8000) != 0;
    }

    bool InputManager::Init()
    {
        LOG_INFO("Initializing InputManager");

        InitKeybinds();

        return true;
    }

    void InputManager::ProcessMousePosition(int& x, int& y) {

        SDL_GetMouseState(&x, &y);
    }

    void InputManager::ProcessInput()
    {

        for (auto& [action, key] : m_InputActions)
        {
            bool bIsPressed = KeyDown(key);
            switch (m_InputActionStates[action])
            {
            case EInputActionState::None:
            {
                m_InputActionStates[action] = bIsPressed ? EInputActionState::JustPressed : EInputActionState::None;
                break;
            }
            case EInputActionState::JustPressed:
            case EInputActionState::Pressed:
            {
                m_InputActionStates[action] = bIsPressed ? EInputActionState::Pressed : EInputActionState::Released;
                break;
            }
            case EInputActionState::Released:
            {
                m_InputActionStates[action] = bIsPressed ? EInputActionState::JustPressed : EInputActionState::None;
                break;
            }
            default:
                ASSERT("Unknown EInputActionState {0}", m_InputActionStates[action]);
                m_InputActionStates[action] = EInputActionState::None;
                break;
            }
        }
    }

    void InputManager::Update(float dt, EntityManager* entityManager)
    {
        ProcessInput();

        int mouse_x, mouse_y;
        ProcessMousePosition(mouse_x, mouse_y);

        // Update entities
        auto inputComponents = entityManager->GetAllComponentInstances<InputComponent>();

        for (auto component : inputComponents)
        {
            component->mouse_x = mouse_x;
            component->mouse_y = mouse_y;

            for (auto& action : component->inputActions)
            {
                action.m_Active = IsButtonActionActive(action.m_Action, action.m_ActionTriggerState);
            }
        }
    }

    void InputManager::Shutdown()
    {
        m_InputActions.clear();
        m_InputActionStates.clear();
    }

    bool InputManager::IsButtonActionActive(EInputAction _eAction, EInputActionState _eState) const
    {
        ASSERT(m_InputActionStates.find(_eAction) != m_InputActionStates.end(), "Unknown input action: {}", _eAction);
        return m_InputActionStates.at(_eAction) == _eState;
    }

    void InputManager::InitKeybinds()
    {
        m_InputActionStates.clear();
        m_InputActions.clear();

        m_InputActions["Player1MoveUp"] = VK_UP;
        m_InputActions["Player1MoveLeft"] = VK_LEFT;
        m_InputActions["Player1MoveDown"] = VK_DOWN;
        m_InputActions["Player1MoveRight"] = VK_RIGHT;
        m_InputActions["Player1RotateTurretCW"] = 'I';
        m_InputActions["Player1RotateTurretCCW"] = 'O';
        m_InputActions["Player1Fire"] = VK_LBUTTON;
        m_InputActions["Player1Special"] = VK_RBUTTON;
        m_InputActions["PauseGame"] = VK_ESCAPE;
        m_InputActions["RestartGame"] = 'R';
        m_InputActions["Player2MoveUp"] = 'W';
        m_InputActions["Player2MoveLeft"] = 'A';
        m_InputActions["Player2MoveDown"] = 'S';
        m_InputActions["Player2MoveRight"] = 'D';
        m_InputActions["Player2Fire"] = 'P';
        m_InputActions["PanCameraUp"] = 'Y';
        m_InputActions["PanCameraLeft"] = 'Y';
        m_InputActions["PanCameraDown"] = 'Y';
        m_InputActions["PanCameraRight"] = 'Y';
        m_InputActions["Menu"] = VK_ESCAPE;
        m_InputActions["Enter"] = VK_RETURN;
        m_InputActions["Test"] = 'X';

    }

    bool InputManager::IsActionActive(InputComponent* inputComponent, EInputAction targetAction)
    {
        auto found = std::find_if(
            std::begin(inputComponent->inputActions),
            std::end(inputComponent->inputActions),
            [targetAction](Engine::InputAction e)
        {
            return e.m_Action == targetAction && e.m_Active == true;
        });

        return found != std::end(inputComponent->inputActions);
    }
}
