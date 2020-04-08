#include "precomp.h"

#include "GameApp.h"
#include "Entities/CameraController.h"

#include <Engine.h>
#include <Core/EntryPoint.h>


void Game::GameApp::GameSpecificWindowData()
{
    Engine::WindowData gameSpecificWindowData;
    gameSpecificWindowData.m_Title = "Real Steel";
    gameSpecificWindowData.m_Width = 1280;
    gameSpecificWindowData.m_Height = 720;
    // gameSpecificWindowData.m_Vsync = true;
    SetWindowData(gameSpecificWindowData);
}

bool Game::GameApp::GameSpecificInit()
{
    m_RenderSystem->SetBackgroundColor(0, 0, 0, 1);

    m_CameraController = std::make_unique<CameraController>();
    m_CameraController->Init(m_EntityManager.get());

    return true;
}

void Game::GameApp::GameSpecificUpdate(float dt)
{
    m_CameraController->Update(dt, m_EntityManager.get());
}

bool Game::GameApp::GameSpecificShutdown()
{
    return true;
}
