#include "precomp.h"

#include "GameApp.h"
#include "Entities/CameraController.h"
#include "Entities/Background.h"
#include "Entities/Player.h"
#include "Entities/HealthBar.h"
#include "Entities/GameComponents.h"
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

    m_CameraController = std::make_unique<CameraController>();
    m_CameraController->Init(m_EntityManager.get());

    //srediti posle...pozadina
    m_Background = std::make_unique<Background>();
    m_TextureManager->CreateTexture(m_RenderSystem->GetRenderer(), "background", "data/textures/grass.png");
    m_Background->Init(m_EntityManager.get(), m_TextureManager->GetTexture("background"));

    m_TextureManager->CreateTexture(m_RenderSystem->GetRenderer(), "player1tank", "data/textures/tank_blue.png");
    m_TextureManager->CreateTexture(m_RenderSystem->GetRenderer(), "player2tank", "data/textures/tank_red.png");

    m_TextureManager->CreateTexture(m_RenderSystem->GetRenderer(), "healthBarRed", "data/textures/HBred.png");
    m_TextureManager->CreateTexture(m_RenderSystem->GetRenderer(), "healthBarFrame", "data/textures/HealthBarFrame.png");

    //player1
    m_Player1 = std::make_unique<Player>(1, vec2(72.f, 250.f), 100.f);
    m_Player1->Init(m_EntityManager.get(), m_TextureManager->GetTexture("player1tank"));
    //player2
    m_Player2 = std::make_unique<Player>(2, vec2(1208.f, 250.f), 100.f);
    m_Player2->Init(m_EntityManager.get(), m_TextureManager->GetTexture("player2tank"));
    //healthbar
    m_HealthBar = std::make_unique<HealthBar>();
    m_HealthBar->Init(m_EntityManager.get(), m_TextureManager->GetTexture("healthBarRed"));
    //health bar frame
    m_HealthBarFrame = std::make_unique<HealthBarFrame>();
    m_HealthBarFrame->Init(m_EntityManager.get(), m_TextureManager->GetTexture("healthBarFrame"));

    //test colider
    auto wall = std::make_unique<Engine::Entity>();
    wall->AddComponent<Engine::TransformComponent>(200.f, 200.f, 40.f, 400.f);
    wall->AddComponent<Engine::SpriteComponent>(m_TextureManager->GetTexture("healthBarRed"));
    wall->AddComponent<Engine::CollisionComponent>(40.f, 400.f);
    m_EntityManager->AddEntity(std::move(wall));

    //AI proba

    m_AI = std::make_unique<AI>();
    m_AI->Init(m_EntityManager.get());

    return true;
}



void Game::GameApp::GameSpecificUpdate(float dt)
{
    m_CameraController->Update(dt, m_EntityManager.get());
    m_Player1->Update(dt, m_EntityManager.get());
    m_Player2->Update(dt, m_EntityManager.get());
    m_HealthBar->Update(dt, m_EntityManager.get());
    m_HealthBarFrame->Update(dt, m_EntityManager.get());
    

    //prepraviti freq update-a AI u nesto sto ima smisla(tipa sekunde)
    if (m_AI_Update_Freq >= 500) {
        
        m_AI_Update_Freq = 0;
        m_AI->Update(dt, m_EntityManager.get());
    }

    m_AI_Update_Freq++;
}

bool Game::GameApp::GameSpecificShutdown()
{
    return true;
}


