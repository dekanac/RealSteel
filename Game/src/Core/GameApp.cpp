#include "precomp.h"

#include "GameApp.h"
#include "PhysicsController.h"
#include "Entities/CameraController.h"
#include "Entities/HealthBar.h"
#include "Entities/GameComponents.h"
#include "Entities/Animation.h"
#include "Entities/StaticObject.h"
#include "Entities/Pickup.h"
#include "Entities/GridSystem.h"
#include "Entities/Tank.h"
#include "Entities/Player.h"
#include "Entities/Bot.h"
#include "Entities/Terrain.h"
#include "InvisibleWallSpawner.h"
#include "Menu/MenuController.h"
#include "Levels/LevelManager.h"
#include "AI/AI.h"

#include <Engine.h>
#include <Core/EntryPoint.h>

#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include <Box2D.h>
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

    b2Vec2 gravity(0.0f, 0.0f);
    //world = new b2World(gravity);

    bool res = InitTextures();
    ASSERT(res, "Textures init fail");
    res = InitSoundsAndMusic();
    ASSERT(res, "Sounds and music init fail");

    m_RenderSystem->GetRenderer()->font = TTF_OpenFont("data/fonts/arial.ttf", 30);


    // Refaktorisati
    auto score = std::make_unique<Engine::Entity>();
    score->AddComponent<Engine::ScoreComponent>();
    auto scoreComp = score->GetComponent<Engine::ScoreComponent>();
    scoreComp->score_num = 0;
    scoreComp->score = std::string("Score: 0");
    score->AddComponent<Engine::TransformComponent>(0,0,0,0);

    m_EntityManager->AddEntity(std::move(score));

    //CONTROLLERS CREATION
    m_PhysicsController = std::make_unique<PhysicsController>();
    m_CameraController = std::make_unique<CameraController>();
    m_HealthBarsController = std::make_unique<HealthBar>();
    m_AnimationsController = std::make_unique<Animation>();
    m_MenuController = std::make_unique<MenuController>();
    m_LevelManager = std::make_unique<LevelManager>();
    m_InvisibleWallSpawner = std::make_unique<InvisibleWallSpawner>();

    //CONTROLLERS INIT -- Redosled je bitan!
    m_PhysicsController->Init(m_EntityManager.get());
    m_CameraController->Init(m_EntityManager.get());
    m_LevelManager->Init(m_EntityManager.get(), m_TextureManager.get(), m_AnimationsController.get()); // Is supposed to draw one of the predefined levels
    m_HealthBarsController->Init(m_EntityManager.get(), m_TextureManager.get());
    m_MenuController->Init(m_EntityManager.get(), m_TextureManager.get());
    m_InvisibleWallSpawner->Init(m_EntityManager.get());


   

    //m_AI = std::make_unique<AI>();
    //m_AI->Init(m_EntityManager.get());

    return true;
}

void Game::GameApp::GameSpecificUpdate(float dt)
{
    
    if (m_GameState == Engine::gameState::RUNNING) {

        m_PhysicsController->Update(dt, m_EntityManager.get());
        m_CameraController->Update(dt, m_EntityManager.get());
        m_HealthBarsController->Update(dt, m_EntityManager.get());
        m_LevelManager->Update(dt, m_EntityManager.get(), m_SoundManager.get(), m_TextureManager.get());
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

            m_LevelManager->ResetLevel(m_EntityManager.get());
            m_GameState = Engine::gameState::RUNNING;
        }
    }
}

bool Game::GameApp::GameSpecificShutdown()
{
    return true;
}


bool Game::GameApp::InitTextures() {

    return
        m_TextureManager->CreateTexture(m_RenderSystem->GetRenderer(), "terrain", "data/textures/static_objects/terrain_bcp.png") &&
        m_TextureManager->CreateTexture(m_RenderSystem->GetRenderer(), "lake", "data/textures/static_objects/lake.png") &&
        m_TextureManager->CreateTexture(m_RenderSystem->GetRenderer(), "tankTurret", "data/textures/tank_turret.png", "data/textures/tank_turret_shadow.png") &&
        m_TextureManager->CreateTexture(m_RenderSystem->GetRenderer(), "tankBody", "data/textures/tank_body.png", "data/textures/tank_body_shadow.png") &&
        m_TextureManager->CreateTexture(m_RenderSystem->GetRenderer(), "healthBarRed", "data/textures/health_bar_red.png") &&
        m_TextureManager->CreateTexture(m_RenderSystem->GetRenderer(), "healthBarFrame", "data/textures/health_bar_frame.png") &&
        m_TextureManager->CreateTexture(m_RenderSystem->GetRenderer(), "bullet", "data/textures/bullet.png") &&
        m_TextureManager->CreateTexture(m_RenderSystem->GetRenderer(), "specialBullet", "data/textures/specialBullet.png") &&
        m_TextureManager->CreateTexture(m_RenderSystem->GetRenderer(), "muzzleFlash", "data/textures/muzzle_flash.png") &&
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
        m_TextureManager->CreateTexture(m_RenderSystem->GetRenderer(), "wallBlockEW", "data/textures/static_objects/wall_EW_block.png", "data/textures/static_objects/wall_EW_block_shadow.png") &&
        m_TextureManager->CreateTexture(m_RenderSystem->GetRenderer(), "wallBlockNS", "data/textures/static_objects/wall_NS_block.png", "data/textures/static_objects/wall_NS_block_shadow.png") &&
        m_TextureManager->CreateTexture(m_RenderSystem->GetRenderer(), "wallBlock", "data/textures/static_objects/wall_block.png", "data/textures/static_objects/wall_block_shadow.png") &&
        m_TextureManager->CreateTexture(m_RenderSystem->GetRenderer(), "palm", "data/textures/static_objects/palm.png", "data/textures/static_objects/palm_shadow.png") &&
        m_TextureManager->CreateTexture(m_RenderSystem->GetRenderer(), "healthPickup", "data/textures/pickups/health.png", "data/textures/pickups/shadow.png") &&
        m_TextureManager->CreateTexture(m_RenderSystem->GetRenderer(), "speedPickup", "data/textures/pickups/speed.png", "data/textures/pickups/shadow.png") &&
        m_TextureManager->CreateTexture(m_RenderSystem->GetRenderer(), "destroyedTank", "data/textures/destroyed_tank.png") &&
        m_TextureManager->CreateTexture(m_RenderSystem->GetRenderer(), "ammunitionPickup", "data/textures/pickups/ammunition.png", "data/textures/pickups/shadow.png") &&
        m_TextureManager->CreateTexture(m_RenderSystem->GetRenderer(), "mainMenu", "data/textures/Main_menu_screen.jpg") &&
        m_TextureManager->CreateTexture(m_RenderSystem->GetRenderer(), "pauseMenu", "data/textures/Pause_menu_screen.jpg") &&
        m_TextureManager->CreateTexture(m_RenderSystem->GetRenderer(), "selectionBox", "data/textures/Selection_box.png") && 
        m_TextureManager->CreateTexture(m_RenderSystem->GetRenderer(), "max_ammunitionPickup", "data/textures/pickups/max_ammunition.png", "data/textures/pickups/shadow.png")&&
        m_TextureManager->CreateTexture(m_RenderSystem->GetRenderer(), "health_helpPickup", "data/textures/pickups/health_help.png", "data/textures/pickups/shadow.png")&&
        m_TextureManager->CreateTexture(m_RenderSystem->GetRenderer(), "missile_powerupPickup", "data/textures/pickups/missile_powerup.png", "data/textures/pickups/shadow.png")&&
        m_TextureManager->CreateTexture(m_RenderSystem->GetRenderer(), "special_missilePickup", "data/textures/pickups/special_missile.png", "data/textures/pickups/shadow.png")&&
        m_TextureManager->CreateTexture(m_RenderSystem->GetRenderer(), "shieldPickup", "data/textures/pickups/shield.png", "data/textures/pickups/shadow.png");
}

bool Game::GameApp::InitSoundsAndMusic() {
    return
        m_SoundManager->AddSound("tankFire", "data/sounds/tank_fire.ogg") &&
        m_SoundManager->AddSound("birdsFly", "data/sounds/birds_fly.ogg") &&
        m_SoundManager->AddSound("tankMove", "data/sounds/tank_move.ogg") &&
        m_SoundManager->AddSound("menuScroll", "data/sounds/menu_scroll.ogg") &&
        m_SoundManager->AddSound("menuConfirm", "data/sounds/menu_confirm.ogg") &&
        m_SoundManager->AddSound("ammunition", "data/sounds/ammunition.ogg") &&
        m_SoundManager->AddSound("speed", "data/sounds/speed.ogg") &&
        m_SoundManager->AddSound("shot", "data/sounds/shot.ogg") &&
        m_SoundManager->AddSound("special", "data/sounds/special_attack.ogg") &&
        m_SoundManager->AddSound("powerup", "data/sounds/powerup.ogg") &&
        m_SoundManager->AddSound("repair", "data/sounds/repair.ogg") &&
        m_SoundManager->AddSound("explosion", "data/sounds/explosion.ogg") &&
        m_SoundManager->AddSound("empty", "data/sounds/empty.ogg") &&
        m_SoundManager->AddSound("shield", "data/sounds/shield.ogg") 
        ;
}