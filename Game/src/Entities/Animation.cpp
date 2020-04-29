#include "precomp.h"
#include "Animation.h"

#include <SDL.h>

namespace Game {

	Engine::Entity* Animation::CreateAnimation(std::string name_, Engine::EntityManager* em_, Engine::TextureManager* tm_) {
		
		ASSERT(tm_ != nullptr, "nullptr passed as textureManager_");
		ASSERT(em_ != nullptr, "nullptr passed as entityManager_");

		auto animationEntity = std::make_unique<Engine::Entity>();
		animationEntity->AddComponent<Engine::TransformComponent>();
		animationEntity->AddComponent<Engine::AnimationComponent>();
		animationEntity->AddComponent<Engine::MoverComponent>();

		auto animationTransf = animationEntity->GetComponent<Engine::TransformComponent>();
		//animationTransf->m_Size = size_;

		auto animationComp = animationEntity->GetComponent<Engine::AnimationComponent>();
		animationComp->m_CurrentRectToDraw = -1;
		animationComp->m_TextureSheet = tm_->GetAnimationTexture({ fmt::format("{}Animation", name_) });

		auto animationPtr = animationEntity.get();
		em_->AddEntity(std::move(animationEntity));

		return animationPtr;
	}
	void Animation::Update(float dt, Engine::EntityManager* entityManager_) {

		auto animationEnts = entityManager_->GetAllEntitiesWithComponents<Engine::TransformComponent, Engine::AnimationComponent>();
		
		for (auto& animation : animationEnts) {

			auto animationTransf = animation->GetComponent<Engine::TransformComponent>();
			auto animationComp = animation->GetComponent<Engine::AnimationComponent>();

			if (animationComp->m_CurrentRectToDraw != -1) {
				animationComp->m_CurrentRectToDraw++;
			}

			if (animationComp->m_CurrentRectToDraw >= animationComp->m_TextureSheet->m_Rects.size()) {
				
				if (animationComp->m_isRepetitive) {
					animationComp->m_CurrentRectToDraw = 0;
				}
				else {
					animationComp->m_CurrentRectToDraw = -1;
				}
			}

		}
	}
}