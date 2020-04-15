#include "precomp.h"
#include "AI.h"


namespace Game {

	bool AI::Init(Engine::EntityManager* entityManager_) {

		return true;
	}

	void AI::Update(float dt, Engine::EntityManager* entityManager_) {
				
	}
	
	vec2 AI::GetPlayerPosition(Engine::EntityManager* entityManager_) {
		auto player = entityManager_->GetAllEntitiesWithComponent<Engine::PlayerComponent>();
		return player[0]->GetComponent<Engine::TransformComponent>()->m_Position;
	}

	float AI::distance(vec2 a, vec2 b) {
		return (b.x - a.x) * (b.x - a.x) + (b.y - a.y) * (b.y - a.y);
	}

	
}
