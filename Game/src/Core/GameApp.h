#pragma once

#include <Core\Application.h>

namespace Game
{
    class CameraController;
    class Terrain;
    class Player;
    class HealthBar;
    class AI;
    class Tank;
    class Animation;
    class StaticObject;
    class Pickup;

    class GameApp final : public Engine::Application
    {
    private:
        void GameSpecificWindowData() override;
        bool GameSpecificInit() override;
        void GameSpecificUpdate(float dt) override;
        bool GameSpecificShutdown() override;

        bool InitTextures();
        bool InitSoundsAndMusic();

        std::unique_ptr<CameraController> m_CameraController{};
        std::unique_ptr<Terrain> m_Terrain{};
        std::unique_ptr<Player> m_PlayersController{};
        std::unique_ptr<HealthBar> m_HealthBarsController{};
        std::unique_ptr<Tank> m_TanksController{};
        std::unique_ptr<AI> m_AI{};
        std::unique_ptr<Animation> m_AnimationsController{};
        std::unique_ptr<StaticObject> m_StaticObjectsController{};
        std::unique_ptr<Pickup> m_PickupsController{};

        int m_AnimationUpdateFreq = 0;
        int m_AIUpdateFreq = 0;

    };

}

Engine::Application* Engine::CreateApplication()
{
    return new Game::GameApp();
}
