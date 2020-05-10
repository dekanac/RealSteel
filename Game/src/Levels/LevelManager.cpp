#include "precomp.h"
#include "LevelManager.h"

#include "Entities/Terrain.h"
#include "Entities/Player.h"
#include "Entities/GameComponents.h"
#include "Entities/Tank.h"
#include "Entities/StaticObject.h"
#include "Entities/Pickup.h"
#include "Entities/GridSystem.h"
#include "Entities/Animation.h"


void Game::LevelManager::Init(Engine::EntityManager* em_, Engine::TextureManager* tm_, Game::Animation* ac_)
{
    m_Terrain = std::make_unique<Terrain>();
    m_TanksController = std::make_unique<Tank>();
    m_PlayersController = std::make_unique<Player>();
    m_StaticObjectsController = std::make_unique<StaticObject>();
    m_PickupsController = std::make_unique<Pickup>();
    m_GridSystem = std::make_unique<GridSystem>();


    //CONTROLLERS INIT
    LoadLevel(1, em_, tm_, ac_, m_GridSystem.get(), m_StaticObjectsController.get()); //TODO: Napraviti da se bira random nivo
    m_Terrain->Init(em_, tm_);
    m_PickupsController->Init(em_, tm_);
    //2 random tenka za probu
    m_TanksController->CreateTank(vec2{ 200.f, 300.f }, em_, tm_);
    m_TanksController->CreateTank(vec2{ 400.f, 300.f }, em_, tm_);
    m_PlayersController->AddPlayer(vec2{ 1100.f, 300.f }, em_, tm_);


}

void Game::LevelManager::Update(float dt,Engine::EntityManager* em_, Engine::SoundManager* sm_)
{
    m_Terrain->Update(dt, em_);
    m_PickupsController->Update(dt, em_, sm_);
    m_TanksController->Update(dt, em_);
    m_PlayersController->Update(dt, em_, sm_);
    m_StaticObjectsController->Update(dt, em_, sm_);
}

void Game::LevelManager::LoadLevel(int level, Engine::EntityManager* em_, Engine::TextureManager* tm_, Game::Animation* ac_, Game::GridSystem* gs_, Game::StaticObject* sc_)
{
    //TODO: Create couple of level layouts
    switch (level) {
    case 1:
    default:   
        //Zidovi
        gs_->createWallHorizontal(130.f, 170.f, 7, em_, tm_, sc_);
        gs_->createWallHorizontal(900.f, 200.f, 7, em_, tm_, sc_);
        gs_->createWallVertical(130.f, 170.f, 3, em_, tm_, sc_);
        gs_->createWallVertical(600.f, 400.f, 5, em_, tm_, sc_);

        //Palme
        gs_->createPalmTree(640.f, 100.f, em_, tm_, sc_, ac_);
        gs_->createPalmTree(380.f, 625.f, em_, tm_, sc_, ac_);
        gs_->createPalmTree(667.f, 625.f, em_, tm_, sc_, ac_);
        gs_->createPalmTree(1000.f, 520.f, em_, tm_, sc_, ac_);
        break;
    case 2:
        break;
    case 3:
        break;
    }
}

void Game::LevelManager::ResetLevel(Engine::EntityManager* em_)
{
    m_PlayersController->Reset(em_);
    m_PickupsController->Reset(em_);
}


