#pragma once

namespace Game {

	class Animation {
	public:
		bool CreateAnimation(Engine::EntityManager* entityManager_, Engine::Texture* texture_, vec2 size_, unsigned int numofParts_);
		void Update(float dt, Engine::EntityManager* entityManager_);
	};

}
