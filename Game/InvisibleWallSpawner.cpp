#include "precomp.h"
#include "invisibleWallSpawner.h"
#include "Entities/GameComponents.h"
#include "Render/WindowData.h"

namespace Game {

	bool InvisibleWallSpawner::CreateInvisibleWall(vec2 pos_, vec2 size_, Engine::EntityManager* entityManager_)
	{
		auto wall = std::make_unique<Engine::Entity>();

		wall->AddComponent<Engine::Box2dBodyComponent>();

		b2BodyDef bodyDef;
		bodyDef.type = b2_staticBody;
		bodyDef.position.Set(pos_.x, pos_.y);

		auto physicsComponent = entityManager_->GetAllComponentInstances<Game::PhysicsComponent>().back();
		wall->GetComponent<Engine::Box2dBodyComponent>()->body = physicsComponent->world->CreateBody(&bodyDef);

		// Define another box shape for our dynamic body.
		b2PolygonShape polygonShape;
		polygonShape.SetAsBox(size_.x, size_.y);
		// Define the dynamic body fixture.
		b2FixtureDef fixtureDef;
		fixtureDef.shape = &polygonShape;
		// Add the shape to the body.
		wall->GetComponent<Engine::Box2dBodyComponent>()->body->CreateFixture(&fixtureDef);

		wall->AddComponent<Engine::TransformComponent>().m_Position = pos_;
		wall->AddComponent<Engine::CollisionComponent>(size_.x, size_.y);

		entityManager_->AddEntity(std::move(wall));

		return true;
	}

	bool InvisibleWallSpawner::Init(Engine::EntityManager* entityManager_) 
	{
		//spawn north wall
		CreateInvisibleWall(vec2(DEFAULT_WIDTH/2, 0), vec2(DEFAULT_WIDTH, 8), entityManager_);
		//spawn west wall
		CreateInvisibleWall(vec2(0, DEFAULT_HEIGHT/2), vec2(8, DEFAULT_HEIGHT), entityManager_);
		//spawn south wall
		CreateInvisibleWall(vec2(DEFAULT_WIDTH / 2, DEFAULT_HEIGHT), vec2(DEFAULT_WIDTH, 8), entityManager_);
		//spawn east wall
		CreateInvisibleWall(vec2(DEFAULT_WIDTH, DEFAULT_HEIGHT / 2), vec2(8, DEFAULT_HEIGHT), entityManager_);

		return true;
	}
}