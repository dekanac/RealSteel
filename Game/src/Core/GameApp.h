#pragma once

#include <Core\Application.h>

namespace Game
{
    class CameraController;
    class Background;
    class Player;
    class HealthBar;
    class AI;
    class Tank;

    class GameApp final : public Engine::Application
    {
    private:
        void GameSpecificWindowData() override;
        bool GameSpecificInit() override;
        void GameSpecificUpdate(float dt) override;
        bool GameSpecificShutdown() override;

        bool InitTextures();
    
        std::unique_ptr<CameraController> m_CameraController{};
        std::unique_ptr<Background> m_Background{};
        std::unique_ptr<Player> m_PlayersController{};
        std::unique_ptr<HealthBar> m_HealthBarsController{};
        std::unique_ptr<AI> m_AI{};
        std::unique_ptr<Tank> m_TanksController{};


        int m_AI_Update_Freq = 0;

    };

}

Engine::Application* Engine::CreateApplication()
{
    return new Game::GameApp();
}
