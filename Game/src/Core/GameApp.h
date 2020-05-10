#pragma once
#include <Box2D.h>
#include <Core\Application.h>

namespace Game
{
    class PhysicsController;
    class CameraController;
    class HealthBar;
    class AI;
    class Animation;
    class MenuController;
    class LevelManager;

    class GameApp final : public Engine::Application
    {
    private:
        void GameSpecificWindowData() override;
        bool GameSpecificInit() override;
        void GameSpecificUpdate(float dt) override;
        bool GameSpecificShutdown() override;

        bool InitTextures();
        bool InitSoundsAndMusic();

        std::unique_ptr<PhysicsController> m_PhysicsController{};
        std::unique_ptr<CameraController> m_CameraController{};
        std::unique_ptr<LevelManager> m_LevelManager{};
        std::unique_ptr<HealthBar> m_HealthBarsController{};
        std::unique_ptr<AI> m_AI{};
        std::unique_ptr<Animation> m_AnimationsController{};
        std::unique_ptr<MenuController> m_MenuController{};

        int m_AnimationUpdateFreq = 0;
        int m_AIUpdateFreq = 0;
    };

}

Engine::Application* Engine::CreateApplication()
{
    return new Game::GameApp();
}
