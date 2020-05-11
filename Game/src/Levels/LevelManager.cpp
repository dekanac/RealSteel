
#include "precomp.h"
#include "LevelManager.h"

#include "Entities/Terrain.h"
#include "Entities/Player.h"
#include "Entities/GameComponents.h"
#include "Entities/Tank.h"
#include "Entities/Bot.h"
#include "Entities/StaticObject.h"
#include "Entities/Pickup.h"
#include "Entities/GridSystem.h"
#include "Entities/Animation.h"
#include "Entities/Bullet.h"

void Game::LevelManager::Init(Engine::EntityManager* em_, Engine::TextureManager* tm_, Game::Animation* ac_)
{
    m_Terrain = std::make_unique<Terrain>();
    m_BotController = std::make_unique<Bot>();
    m_TanksController = std::make_unique<Tank>();
    m_PlayersController = std::make_unique<Player>();
    m_StaticObjectsController = std::make_unique<StaticObject>();
    m_PickupsController = std::make_unique<Pickup>();
    m_GridSystem = std::make_unique<GridSystem>();


    //CONTROLLERS INIT
    m_GridSystem->Init();
    auto randLevel = rand() % 3 + 1;
    LoadLevel(randLevel, em_, tm_, ac_, m_GridSystem.get(), m_StaticObjectsController.get()); //TODO: Ubaciti randLevel kao prvi parametar kad se odrade ostali tereni
    m_Terrain->Init(em_, tm_);
    m_PickupsController->Init(em_, tm_);
    
}

void Game::LevelManager::Update(float dt,Engine::EntityManager* em_, Engine::SoundManager* sm_, Engine::TextureManager* tm_)
{
    m_Terrain->Update(dt, em_);
    m_PickupsController->Update(dt, em_, sm_, m_GridSystem.get());
    m_BotController->Update(dt, em_, m_GridSystem.get());
    m_TanksController->Update(dt, em_, tm_, sm_);
    m_PlayersController->Update(dt, em_, sm_, tm_);    
    m_StaticObjectsController->Update(dt, em_, sm_);
    Game::Bullet::Update(dt, em_, sm_);

    // Refaktorisati
    auto xs = em_->GetAllEntitiesWithComponent<Engine::ScoreComponent>();
    if (xs.size() != 0)
    {
        auto x = xs.at(0);
        auto s = x->GetComponent<Engine::ScoreComponent>();

        s->score = fmt::format("Score: {}", s->score_num);
    }

}

void Game::LevelManager::LoadLevel(int level, Engine::EntityManager* em_, Engine::TextureManager* tm_, Game::Animation* ac_, Game::GridSystem* gs_, Game::StaticObject* sc_)
{
    auto imageEntity = std::make_unique<Engine::Entity>();
    imageEntity->AddComponent<Engine::TransformComponent>(DEFAULT_WIDTH/2, DEFAULT_HEIGHT/2, 1280.f, 720.f);
    imageEntity->AddComponent<Engine::TerrainComponent>().m_Texture = tm_->GetTexture("terrain");

    em_->AddEntity(std::move(imageEntity));

    //TODO: Create couple of level layouts
    switch (level) {
    case 1:
    default:   
        //Zidovi
        //gs_->createWallHorizontal(10.f, 10.f, 32, em_, tm_, sc_);
        //gs_->createWallHorizontal(10.f, 720.f, 32, em_, tm_, sc_);
        //gs_->createWallVertical(10.f, 10.f, 18, em_, tm_, sc_);
        //gs_->createWallVertical(1250.f, 10.f, 18, em_, tm_, sc_);

        gs_->createWallVertical(130.f, 10.f, 4, em_, tm_, sc_);
        gs_->createWallVertical(1130.f, 10.f, 4, em_, tm_, sc_);

        gs_->createWallHorizontal(130.f, 130.f, 4, em_, tm_, sc_);
        gs_->createWallHorizontal(1010.f, 130.f, 4, em_, tm_, sc_);

        gs_->createWallHorizontal(130.f, 370.f, 4, em_, tm_, sc_);
        gs_->createWallHorizontal(130.f, 330.f, 4, em_, tm_, sc_);

        gs_->createWallHorizontal(1010.f, 370.f, 4, em_, tm_, sc_);
        gs_->createWallHorizontal(1010.f, 330.f, 4, em_, tm_, sc_);

        gs_->createWallHorizontal(130.f, 570.f, 4, em_, tm_, sc_);
        gs_->createWallHorizontal(1010.f, 570.f, 4, em_, tm_, sc_);

        gs_->createWallVertical(130.f, 570, 4, em_, tm_, sc_);
        gs_->createWallVertical(1130.f, 570, 4, em_, tm_, sc_);

        gs_->createWallVertical(490.f, 130, 12, em_, tm_, sc_);
        gs_->createWallVertical(750.f, 130, 12, em_, tm_, sc_);

        gs_->createLake(75.f, 630.f, 370.f, em_, tm_, sc_);
        gs_->createLake(0.f, 880.f, 210.f, em_, tm_, sc_);
      
        //2 random tenka za probu
        m_BotController->AddBot(vec2{ 200.f, 250.f }, em_, tm_);
        m_BotController->AddBot(vec2{ 200.f, 470.f }, em_, tm_);
        m_PlayersController->AddPlayer(vec2{ 1100.f, 250.f }, em_, tm_);
        
        //Palme
        gs_->createPalmTree(670.f, 370.f, em_, tm_, sc_, ac_);
        gs_->createPalmTree(380.f, 50.f, em_, tm_, sc_, ac_);
        gs_->createPalmTree(1080.f, 480.f, em_, tm_, sc_, ac_);
        gs_->createPalmTree(420.f, 480.f, em_, tm_, sc_, ac_);
        gs_->createPalmTree(880.f, 170.f, em_, tm_, sc_, ac_);

        break;
    case 2:
        //Zidovi
        gs_->createWallHorizontal(130.f, 170.f, 7, em_, tm_, sc_);
        gs_->createWallHorizontal(900.f, 200.f, 7, em_, tm_, sc_);
        gs_->createWallVertical(130.f, 170.f, 3, em_, tm_, sc_);
        gs_->createWallVertical(600.f, 400.f, 5, em_, tm_, sc_);
        //2 random tenka za probu
        m_BotController->AddBot(vec2{ 200.f, 250.f }, em_, tm_);
        m_BotController->AddBot(vec2{ 200.f, 470.f }, em_, tm_);
        m_PlayersController->AddPlayer(vec2{ 1100.f, 250.f }, em_, tm_);

        //Palme
        gs_->createPalmTree(640.f, 100.f, em_, tm_, sc_, ac_);
        gs_->createPalmTree(380.f, 625.f, em_, tm_, sc_, ac_);
        gs_->createPalmTree(667.f, 625.f, em_, tm_, sc_, ac_);
        gs_->createPalmTree(1080.f, 520.f, em_, tm_, sc_, ac_);
        
        break;
    case 3: //Zidovi
        gs_->createWallHorizontal(130.f, 170.f, 7, em_, tm_, sc_);
        gs_->createWallHorizontal(900.f, 200.f, 7, em_, tm_, sc_);
        gs_->createWallVertical(130.f, 170.f, 3, em_, tm_, sc_);
        gs_->createWallVertical(600.f, 400.f, 5, em_, tm_, sc_);
        //2 random tenka za probu
        m_BotController->AddBot(vec2{ 200.f, 250.f }, em_, tm_);
        m_BotController->AddBot(vec2{ 200.f, 470.f }, em_, tm_);
        m_PlayersController->AddPlayer(vec2{ 1100.f, 250.f }, em_, tm_);

        //Palme
        gs_->createPalmTree(640.f, 100.f, em_, tm_, sc_, ac_);
        gs_->createPalmTree(380.f, 625.f, em_, tm_, sc_, ac_);
        gs_->createPalmTree(667.f, 625.f, em_, tm_, sc_, ac_);
        gs_->createPalmTree(1000.f, 520.f, em_, tm_, sc_, ac_);
        
        break;
    }
}

void Game::LevelManager::ResetLevel(Engine::EntityManager* em_)
{
    m_PlayersController->Reset(em_);
    m_PickupsController->Reset(em_);
    m_BotController->Reset(em_);
}


