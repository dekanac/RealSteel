#include "precomp.h"

#include "GameApp.h"
#include "Entities/CameraController.h"
#include "Entities/Background.h"
#include "Entities/Player.h"
#include "Entities/HealthBar.h"

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

    m_TextureManager->CreateTexture(m_RenderSystem->GetRenderer(), "player1tank", "data/textures/tank.png");
    m_TextureManager->CreateTexture(m_RenderSystem->GetRenderer(), "healthBarRed", "data/textures/HBred.png");

    //player1
    m_Player1 = std::make_unique<Player>(1, vec2(550.f, 250.f), 100.f);
    m_Player1->Init(m_EntityManager.get(), m_TextureManager->GetTexture("player1tank"));
    //player2
    m_Player2 = std::make_unique<Player>(2, vec2(-550.f, 250.f), 100.f);
    m_Player2->Init(m_EntityManager.get(), m_TextureManager->GetTexture("player1tank"));
    //healthbar
    m_HealthBar = std::make_unique<HealthBar>();
    m_HealthBar->Init(m_EntityManager.get(), m_TextureManager->GetTexture("healthBarRed"));

    return true;
}

void Game::GameApp::GameSpecificUpdate(float dt)
{
    m_CameraController->Update(dt, m_EntityManager.get());   
    m_Player1->Update(dt, m_EntityManager.get());
    m_Player2->Update(dt, m_EntityManager.get());
    m_HealthBar->Update(dt, m_EntityManager.get());
}

bool Game::GameApp::GameSpecificShutdown()
{
    return true;
}
