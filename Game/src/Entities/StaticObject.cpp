#include "precomp.h"
#include "StaticObject.h"
#include "GameComponents.h"
#include "Animation.h"

namespace Game {

	bool StaticObject::CreateWallBlock(std::string type_, vec2 pos_, Engine::EntityManager* entityManager_, Engine::TextureManager* textureManager_)
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
		polygonShape.SetAsBox(20, 20);
		// Define the dynamic body fixture.
		b2FixtureDef fixtureDef;
		fixtureDef.shape = &polygonShape;
		// Add the shape to the body.
		wall->GetComponent<Engine::Box2dBodyComponent>()->body->CreateFixture(&fixtureDef);

		wall->AddComponent<Engine::TransformComponent>().m_Position = pos_;
		wall->AddComponent<Engine::SpriteComponent>(textureManager_->GetTexture({ fmt::format("wallBlock{}", type_) }));
		wall->AddComponent<Game::WallComponent>();
		wall->AddComponent<Game::SolidObjectComponent>();
		//wall->AddComponent<Engine::ShadowComponent>();
		wall->AddComponent<Engine::CollisionComponent>(40.f, 40.f);

		entityManager_->AddEntity(std::move(wall));

		return true;
	}

	bool StaticObject::CreatePalm(vec2 pos_, Engine::EntityManager* entityManager_, Engine::TextureManager* textureManager_, Game::Animation* animationsController_) {

		auto palm = std::make_unique<Engine::Entity>();
		palm->AddComponent<Engine::TransformComponent>().m_Position = pos_;
		
		palm->AddComponent<Engine::SpriteComponent>(textureManager_->GetTexture("palm"));
		palm->AddComponent<Engine::ShadowComponent>();
		palm->AddComponent<Engine::CollisionComponent>(40.f, 40.f);
		palm->AddComponent<Game::SolidObjectComponent>();
		palm->AddComponent<Game::PalmComponent>();
		auto palmComp = palm->GetComponent<Game::PalmComponent>();
		 
		palmComp->birdsAnimation = animationsController_->CreateAnimation("birds", entityManager_, textureManager_);

		auto animTransf = palmComp->birdsAnimation->GetComponent<Engine::TransformComponent>();
		animTransf->m_Rotation = 180.f;
		animTransf->m_Position = vec2{ -100.f, -100.f };
		animTransf->m_Size = vec2{ 40.f, 40.f };
		auto animAnimComp = palmComp->birdsAnimation->GetComponent<Engine::AnimationComponent>();
		animAnimComp->m_isRepetitive = true;

		entityManager_->AddEntity(std::move(palm));

		return true;
	}
	
	bool StaticObject::CreateLake(float rotation_, vec2 pos_, Engine::EntityManager* entityManager_, Engine::TextureManager* textureManager_) {

		auto lake = std::make_unique<Engine::Entity>();
		lake->AddComponent<Engine::TransformComponent>();

		auto lakeTransf = lake->GetComponent<Engine::TransformComponent>();
		lakeTransf->m_Position = pos_;
		lakeTransf->m_Rotation = rotation_;

		//za potrebe collision boxa crtati sa sprite-om
		//lake->AddComponent<Engine::SpriteComponent>(textureManager_->GetTexture("lake"));
		lake->AddComponent<Engine::CollisionComponent>(120.f, 120.f);

		lake->AddComponent<Engine::TerrainComponent>().m_Texture = textureManager_->GetTexture("lake");
		lake->AddComponent<Game::LakeComponent>();

		entityManager_->AddEntity(std::move(lake));

		return true;
	}


	void StaticObject::Update(float dt, Engine::EntityManager* entityManager_, Engine::SoundManager* soundManager_) {

		auto solidObjects = entityManager_->GetAllEntitiesWithComponents<Game::SolidObjectComponent>();

		for (auto& solidObject : solidObjects) {

			//UPDATE PALMI
			if (solidObject->HasComponent<Game::PalmComponent>()) {

				auto palmCollisionComp = solidObject->GetComponent<Engine::CollisionComponent>();

				if (!(palmCollisionComp->m_CollidedWith.empty())) {

					auto palmPalmComp = solidObject->GetComponent<Game::PalmComponent>();

					//animacija sa pticurinama
					if (palmPalmComp->m_Birds) {

						auto birdsTranf = palmPalmComp->birdsAnimation->GetComponent<Engine::TransformComponent>();
						auto palmTransf = solidObject->GetComponent<Engine::TransformComponent>();

						birdsTranf->m_Position = palmTransf->m_Position;
						birdsTranf->m_Rotation = rand() % 360;
						auto angleRad = (birdsTranf->m_Rotation * 3.14f) / 180.f;

						auto birdMove = palmPalmComp->birdsAnimation->GetComponent<Engine::MoverComponent>();
						birdMove->m_TranslationSpeed = vec2{ sin(angleRad) * 300.f, -cos(angleRad) * 300.f };

						auto birdsStart = palmPalmComp->birdsAnimation->GetComponent<Engine::AnimationComponent>();
						birdsStart->m_CurrentRectToDraw = 0;

						soundManager_->PlaySound("birdsFly", 0);
						palmPalmComp->m_Birds = false;

					}

				}
			}
		}

	}
}