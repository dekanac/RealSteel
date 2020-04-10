#include "precomp.h"

#include "GameApp.h"
#include "Entities/CameraController.h"
#include "Entities/Background.h"
#include "Entities/Player.h"

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

    //srediti posle...pozadina
    m_Background = std::make_unique<Background>();
    m_TextureManager->CreateTexture(m_RenderSystem->GetRenderer(), "background", "data/textures/grass.png");
    m_Background->Init(m_EntityManager.get(), m_TextureManager->GetTexture("background"));


    //BAG: dok je na playeru 1 pritisnuto GORE LEVO na playeru 2 ne rade kontrole kako treba
    //player1
    m_Player1 = std::make_unique<Player>(1, vec2(550.f, 250.f));
    m_TextureManager->CreateTexture(m_RenderSystem->GetRenderer(), "player", "data/textures/tank.png");
    m_Player1->Init(m_EntityManager.get(), m_TextureManager->GetTexture("player"));
    //player2
    m_Player2 = std::make_unique<Player>(2, vec2(-550.f, 250.f), 200.f);
    m_Player2->Init(m_EntityManager.get(), m_TextureManager->GetTexture("player"));


    return true;
}

void Game::GameApp::GameSpecificUpdate(float dt)
{
    m_CameraController->Update(dt, m_EntityManager.get());   
    m_Player1->Update(dt, m_EntityManager.get());
    m_Player2->Update(dt, m_EntityManager.get());
}

bool Game::GameApp::GameSpecificShutdown()
{
    return true;
}
