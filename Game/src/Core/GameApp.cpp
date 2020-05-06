#include "precomp.h"

#include "GameApp.h"
#include "Entities/CameraController.h"
#include "Entities/Terrain.h"
#include "Entities/Player.h"
#include "Entities/HealthBar.h"
#include "Entities/GameComponents.h"
#include "Entities/Tank.h"
#include "Entities/Animation.h"
#include "Entities/StaticObject.h"
#include "Entities/Pickup.h"
#include "Menu/MenuController.h"
#include "AI/AI.h"


#include <Engine.h>
#include <Core/EntryPoint.h>

#include <SDL_mixer.h>
#include <ctime>


void Game::GameApp::GameSpecificWindowData()
{
    Engine::WindowData gameSpecificWindowData;
    gameSpecificWindowData.m_Title = "Real Steel";
    gameSpecificWindowData.m_Width = DEFAULT_WIDTH;
    gameSpecificWindowData.m_Height = DEFAULT_HEIGHT;
    gameSpecificWindowData.m_Vsync = true;
    SetWindowData(gameSpecificWindowData);
}

bool Game::GameApp::GameSpecificInit()
{
    m_RenderSystem->SetBackgroundColor(0, 0, 0, 1);
    
    game_start_time = time(0);
    srand(game_start_time);

    bool res = InitTextures();
    ASSERT(res, "Textures init fail");
    res = InitSoundsAndMusic();
    ASSERT(res, "Sounds and music init fail");

    //CONTROLLERS CREATION
    m_CameraController = std::make_unique<CameraController>();
    m_Terrain = std::make_unique<Terrain>();
    m_TanksController = std::make_unique<Tank>();
    m_PlayersController = std::make_unique<Player>();
    m_HealthBarsController = std::make_unique<HealthBar>();
    m_AnimationsController = std::make_unique<Animation>();
    m_StaticObjectsController = std::make_unique<StaticObject>();
    m_PickupsController = std::make_unique<Pickup>();
    m_MenuController = std::make_unique<MenuController>();
    

    //CONTROLLERS INIT
    m_CameraController->Init(m_EntityManager.get());
    m_Terrain->Init(m_EntityManager.get(), m_TextureManager.get());
    m_PickupsController->Init(m_EntityManager.get(), m_TextureManager.get());
    //2 random tenka za probu
    m_TanksController->CreateTank(vec2{ 200.f, 300.f }, m_EntityManager.get(), m_TextureManager.get());
    m_TanksController->CreateTank(vec2{ 400.f, 300.f }, m_EntityManager.get(), m_TextureManager.get());
    m_PlayersController->AddPlayer(vec2{ 1100.f, 300.f }, m_EntityManager.get(), m_TextureManager.get());
    m_StaticObjectsController->DrawWorld(m_EntityManager.get(), m_TextureManager.get(), m_AnimationsController.get());
    m_HealthBarsController->Init(m_EntityManager.get(), m_TextureManager.get());
    //Menu
    m_MenuController->Init(m_EntityManager.get(), m_TextureManager.get());

    //m_AI = std::make_unique<AI>();
    //m_AI->Init(m_EntityManager.get());

    return true;
}

void Game::GameApp::GameSpecificUpdate(float dt)
{
    
    if (m_GameState == Engine::gameState::RUNNING) {
        m_CameraController->Update(dt, m_EntityManager.get());
        m_PlayersController->Update(dt, m_EntityManager.get(), m_SoundManager.get());
        m_HealthBarsController->Update(dt, m_EntityManager.get());
        m_TanksController->Update(dt, m_EntityManager.get());
        m_PickupsController->Update(dt, m_EntityManager.get(), m_SoundManager.get());
        m_StaticObjectsController->Update(dt, m_EntityManager.get(), m_SoundManager.get());
        m_Terrain->Update(dt, m_EntityManager.get());
        m_GameState = m_MenuController->Update(dt, m_EntityManager.get(), m_SoundManager.get(), m_CameraController.get());
        
        if (m_AnimationUpdateFreq >= 4) {
            m_AnimationUpdateFreq = 0;
            m_AnimationsController->Update(dt, m_EntityManager.get());
        }

        //if (m_AIUpdateFreq >= 10) {
            //m_AIUpdateFreq = 0;
            //m_AI->Update(dt, m_EntityManager.get());
        //}

        m_AnimationUpdateFreq++;
        m_AIUpdateFreq++;
    }
    else if (m_GameState == Engine::gameState::IN_MENU or m_GameState == Engine::gameState::PAUSED) {
        m_GameState = m_MenuController->Update(dt, m_EntityManager.get(), m_SoundManager.get(), m_CameraController.get());
        if (m_GameState == Engine::gameState::RESTART) {

            ResetLevel();
            m_GameState = Engine::gameState::RUNNING;
        }
    }
}

bool Game::GameApp::GameSpecificShutdown()
{
    return true;
}

void Game::GameApp::ResetLevel()
{
    //TODO: Reset level
}

bool Game::GameApp::InitTextures() {

    return
        m_TextureManager->CreateTexture(m_RenderSystem->GetRenderer(), "terrain", "data/textures/static_objects/terrain.png") &&
        m_TextureManager->CreateTexture(m_RenderSystem->GetRenderer(), "lake", "data/textures/static_objects/lake.png") &&
        m_TextureManager->CreateTexture(m_RenderSystem->GetRenderer(), "tankTurret", "data/textures/tank_turret.png", "data/textures/tank_turret_shadow.png") &&
        m_TextureManager->CreateTexture(m_RenderSystem->GetRenderer(), "tankBody", "data/textures/tank_body.png", "data/textures/tank_body_shadow.png") &&
        m_TextureManager->CreateTexture(m_RenderSystem->GetRenderer(), "healthBarRed", "data/textures/health_bar_red.png") &&
        m_TextureManager->CreateTexture(m_RenderSystem->GetRenderer(), "healthBarFrame", "data/textures/health_bar_frame.png") &&
        m_TextureManager->CreateAnimationTexture(
            vec2{ 200, 200 }, 
            4, 
            m_RenderSystem->GetRenderer(), 
            "explosionAnimation", 
            "data/textures/explosion_animation.png") &&
        m_TextureManager->CreateAnimationTexture(
            vec2{ 192, 192 },
            20,                                      
            m_RenderSystem->GetRenderer(), 
            "birdsAnimation", 
            "data/textures/bird_animation.png") &&
        m_TextureManager->CreateTexture(m_RenderSystem->GetRenderer(), "wallEW", "data/textures/static_objects/wall_EW.png", "data/textures/static_objects/wall_EW_shadow.png") &&
        m_TextureManager->CreateTexture(m_RenderSystem->GetRenderer(), "wallNS", "data/textures/static_objects/wall_NS.png", "data/textures/static_objects/wall_NS_shadow.png") &&
        m_TextureManager->CreateTexture(m_RenderSystem->GetRenderer(), "palm", "data/textures/static_objects/palm.png", "data/textures/static_objects/palm_shadow.png") &&
        m_TextureManager->CreateTexture(m_RenderSystem->GetRenderer(), "healthPickup", "data/textures/pickups/health.png", "data/textures/pickups/shadow.png") &&
        m_TextureManager->CreateTexture(m_RenderSystem->GetRenderer(), "speedPickup", "data/textures/pickups/speed.png", "data/textures/pickups/shadow.png") &&
        m_TextureManager->CreateTexture(m_RenderSystem->GetRenderer(), "destroyedTank", "data/textures/destroyed_tank.png") &&
        m_TextureManager->CreateTexture(m_RenderSystem->GetRenderer(), "ammunitionPickup", "data/textures/pickups/ammunition.png", "data/textures/pickups/shadow.png") &&
        m_TextureManager->CreateTexture(m_RenderSystem->GetRenderer(), "mainMenu", "data/textures/Main_menu_screen.jpg") &&
        m_TextureManager->CreateTexture(m_RenderSystem->GetRenderer(), "pauseMenu", "data/textures/Pause_menu_screen.jpg") &&
        m_TextureManager->CreateTexture(m_RenderSystem->GetRenderer(), "selectionBox", "data/textures/Selection_box.png");
}

bool Game::GameApp::InitSoundsAndMusic() {
    return
        m_SoundManager->AddSound("tankFire", "data/sounds/tank_fire.ogg") &&
        m_SoundManager->AddSound("birdsFly", "data/sounds/birds_fly.ogg") &&
        m_SoundManager->AddSound("pickup", "data/sounds/pickup_voice.ogg") &&
        m_SoundManager->AddSound("tankMove", "data/sounds/tank_move.ogg") &&
        m_SoundManager->AddSound("menuScroll", "data/sounds/menu_scroll.ogg") &&
        m_SoundManager->AddSound("menuConfirm", "data/sounds/menu_confirm.ogg");
}