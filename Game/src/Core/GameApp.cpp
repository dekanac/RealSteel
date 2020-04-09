#include "precomp.h"

#include "GameApp.h"
#include "Entities/CameraController.h"

#include <Engine.h>
#include <Core/EntryPoint.h>

#include <SDL_mixer.h>


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

    //sound test
    //pritisni 1 ali prvo stisaj zvucnik
    //pritisni 9 za pustanje muzike 7 za pauzu 6 za resume 0 za stop
    //TODO: obrisati iz Application.cpp glavnog loop-a celu petlju za input
    m_SoundManager->AddSound("fire", "data/sounds/fire.wav");
    m_SoundManager->AddMusic("music_test", "data/music/music_test.ogg");

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
