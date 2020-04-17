#include "precomp.h"

#include "GameApp.h"
#include "Entities/CameraController.h"
#include "Entities/Background.h"
#include "Entities/Player.h"
#include "Entities/HealthBar.h"
#include "Entities/GameComponents.h"
#include "Entities/Tank.h"
#include "AI/AI.h"

#include <Engine.h>
#include <Core/EntryPoint.h>

#include <SDL_mixer.h>


void Game::GameApp::GameSpecificWindowData()
{
    Engine::WindowData gameSpecificWindowData;
    gameSpecificWindowData.m_Title = "Real Steel";
    gameSpecificWindowData.m_Width = DEFAULT_WIDTH;
    gameSpecificWindowData.m_Height = DEFAULT_HEIGHT;
    // gameSpecificWindowData.m_Vsync = true;
    SetWindowData(gameSpecificWindowData);
}

bool Game::GameApp::GameSpecificInit()
{
    m_RenderSystem->SetBackgroundColor(0, 0, 0, 1);
    ASSERT(InitTextures(), "Textures init fail");

    m_CameraController = std::make_unique<CameraController>();
    m_Background = std::make_unique<Background>();
    m_TanksController = std::make_unique<Tank>();
    m_PlayersController = std::make_unique<Player>();
    m_HealthBarsController = std::make_unique<HealthBar>();

    m_CameraController->Init(m_EntityManager.get());
    m_Background->Init(m_EntityManager.get(), m_TextureManager->GetTexture("background"));
    //2 random tenka za probu
    m_TanksController->CreateTank(vec2{ 200.f, 300.f }, m_EntityManager.get(), m_TextureManager.get());
    m_TanksController->CreateTank(vec2{ 400.f, 300.f }, m_EntityManager.get(), m_TextureManager.get());
    m_PlayersController->AddPlayer(vec2{ 1000.f, 300.f }, m_EntityManager.get(), m_TextureManager.get());
    m_PlayersController->AddPlayer(vec2{ 800.f, 300.f }, m_EntityManager.get(), m_TextureManager.get());
    m_HealthBarsController->Init(m_EntityManager.get(), m_TextureManager.get());
    

    //m_AI = std::make_unique<AI>();
    //m_AI->Init(m_EntityManager.get());

    return true;
}



void Game::GameApp::GameSpecificUpdate(float dt)
{
    m_CameraController->Update(dt, m_EntityManager.get());
    m_PlayersController->Update(dt, m_EntityManager.get());
    m_HealthBarsController->Update(dt, m_EntityManager.get());
    m_TanksController->Update(dt, m_EntityManager.get());
    
    
    //prepraviti freq update-a AI u nesto sto ima smisla(tipa sekunde)
    /*
    if (m_AI_Update_Freq >= 500) {
        
        m_AI_Update_Freq = 0;
        m_AI->Update(dt, m_EntityManager.get());
    }

    m_AI_Update_Freq++;
    */
}

bool Game::GameApp::GameSpecificShutdown()
{
    return true;
}

bool Game::GameApp::InitTextures() {

    return 
    m_TextureManager->CreateTexture(m_RenderSystem->GetRenderer(), "background", "data/textures/grass.png") &&
    m_TextureManager->CreateTexture(m_RenderSystem->GetRenderer(), "tankturret", "data/textures/tank_turret.png") &&
    m_TextureManager->CreateTexture(m_RenderSystem->GetRenderer(), "tankbody", "data/textures/tank_body.png") &&
    m_TextureManager->CreateTexture(m_RenderSystem->GetRenderer(), "player1tank", "data/textures/tank_blue.png") &&
    m_TextureManager->CreateTexture(m_RenderSystem->GetRenderer(), "player2tank", "data/textures/tank_red.png") &&
    m_TextureManager->CreateTexture(m_RenderSystem->GetRenderer(), "healthBarRed", "data/textures/HBred.png") &&
    m_TextureManager->CreateTexture(m_RenderSystem->GetRenderer(), "healthBarFrame", "data/textures/HealthBarFrame.png");

}

