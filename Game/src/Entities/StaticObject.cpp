#include "precomp.h"
#include "StaticObject.h"
#include "GameComponents.h"

namespace Game {
	
	//drawing all static objects in world
	bool StaticObject::DrawWorld(Engine::EntityManager* em_, Engine::TextureManager* tm_) {
		
		CreateWall("NS", vec2{ 300, 300.f }, em_, tm_);
		CreateWall("EW", vec2{ 300.f - 46.f/2.f + 169.f/2.f + 4.f, 300.f - 169.f/2.f - 46.f/2.f + 4.f }, em_, tm_);
			
		CreateWall("NS", vec2{ 980.f, 300.f }, em_, tm_);
		CreateWall("EW", vec2{ 980.f + 46.f/2.f - 169.f/2.f - 4.f, 300.f - 169.f/2.f - 46.f/2.f + 4.f }, em_, tm_);
		
		CreateWall("EW", vec2{ 640.f, 360.f }, em_, tm_);
		CreatePalm(vec2{ 640.f, 100.f }, em_, tm_);

		
		
		return true;
	}

	bool StaticObject::CreateWall(std::string type_, vec2 pos_, Engine::EntityManager* entityManager_, Engine::TextureManager* textureManager_) {

		auto wall = std::make_unique<Engine::Entity>();
		wall->AddComponent<Engine::TransformComponent>().m_Position = pos_;
		
		wall->AddComponent<Engine::SpriteComponent>(textureManager_->GetTexture({fmt::format("wall{}", type_)}));
		wall->AddComponent<Game::WallComponent>();
		wall->AddComponent<Engine::ShadowComponent>();
		wall->AddComponent<Engine::CollisionComponent>(
				type_ == "EW" ? WALL_SIZE_X : WALL_SIZE_Y,
				type_ == "EW" ? WALL_SIZE_Y : WALL_SIZE_X   
			);

		entityManager_->AddEntity(std::move(wall));

		return true;
	}
	bool StaticObject::CreatePalm(vec2 pos_, Engine::EntityManager* entityManager_, Engine::TextureManager* textureManager_) {

		auto palm = std::make_unique<Engine::Entity>();
		palm->AddComponent<Engine::TransformComponent>().m_Position = pos_;
		
		palm->AddComponent<Engine::SpriteComponent>(textureManager_->GetTexture("palm"));
		palm->AddComponent<Game::WallComponent>();
		palm->AddComponent<Engine::ShadowComponent>();
		palm->AddComponent<Engine::CollisionComponent>(80.f);

		entityManager_->AddEntity(std::move(palm));

		return true;
	}
}