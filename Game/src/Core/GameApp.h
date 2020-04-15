#pragma once

#include <Core\Application.h>

namespace Game
{
    class CameraController;
    class Background;
    class Player;
    class HealthBar;
    class HealthBarFrame;
    class AI;

    class GameApp final : public Engine::Application
    {
    private:
        void GameSpecificWindowData() override;
        bool GameSpecificInit() override;
        void GameSpecificUpdate(float dt) override;
        bool GameSpecificShutdown() override;
    
        std::unique_ptr<CameraController> m_CameraController{};
        std::unique_ptr<Background> m_Background{};
        std::unique_ptr<Player> m_Player1{};
        std::unique_ptr<Player> m_Player2{};
        std::unique_ptr<HealthBar> m_HealthBar{};
        std::unique_ptr<HealthBarFrame> m_HealthBarFrame{};
        std::unique_ptr<AI> m_AI{};

        int m_AI_Update_Freq = 0;

    };

}

Engine::Application* Engine::CreateApplication()
{
    return new Game::GameApp();
}
