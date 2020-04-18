#include "precomp.h"
#include "Animation.h"

#include <SDL.h>

namespace Game {

	bool Animation::CreateAnimation(Engine::EntityManager* entityManager_, Engine::Texture* texture_, vec2 size_, unsigned int numofParts_) {
		
		auto animationEntity = std::make_unique<Engine::Entity>();
		animationEntity->AddComponent<Engine::TransformComponent>();
		animationEntity->AddComponent<Engine::AnimationComponent>();
		
		auto animationTransf = animationEntity->GetComponent<Engine::TransformComponent>();
		animationTransf->m_Size = size_;
		animationTransf->m_Position = vec2{ -1000.f, -1000.f };

		auto animationComp = animationEntity->GetComponent<Engine::AnimationComponent>();
		animationComp->m_CurrentRectToDraw = -1;
		animationComp->m_TextureSheet = texture_;
		auto rects = std::vector<SDL_Rect>{ numofParts_ };

		//pravimo %d,(numOfParts_) SDL_Rect-ova kako bi iz slike teksture pokupili vise manjih tekstura
		int w, h;
		SDL_QueryTexture(texture_->m_Texture, NULL, NULL, &w, &h);
		int num_w = w / size_.x; //broj tekstura po horizontali
		int num_h = h / size_.y; //broj tekstura po vertikali

		//counter je izlazak iz petlje ako imamo recimo 7 delova teksture u matrici 3x3 (ne ucitavamo posled. dve)
		int counter = 0;
		for (int i = 0; i < num_h; i++) {
			for (int j = 0; j < num_w; j++) {

				rects[counter] = SDL_Rect{
					j * (int)size_.x,
					i * (int)size_.y,
					(int)size_.x,
					(int)size_.y,
				};
				counter++;
				if (counter == numofParts_) {
					break;
				}
			}
		}
		
		animationComp->m_Rects = rects;

		entityManager_->AddEntity(std::move(animationEntity));

		return true;
	}
	void Animation::Update(float dt, Engine::EntityManager* entityManager_) {

		auto animationEnts = entityManager_->GetAllEntitiesWithComponents<Engine::TransformComponent, Engine::AnimationComponent>();
		
		for (auto& animation : animationEnts) {

			auto animationTransf = animation->GetComponent<Engine::TransformComponent>();
			auto animationComp = animation->GetComponent<Engine::AnimationComponent>();

			//animationTransf->m_Size = vec2{ 50.f, 50.f };
			animationTransf->m_Position = vec2{ 600.f, 370.f };

			if (animationComp->m_CurrentRectToDraw != -1) {
				animationComp->m_CurrentRectToDraw++;
			}

			if (animationComp->m_CurrentRectToDraw >= animationComp->m_Rects.size()) {
				animationComp->m_CurrentRectToDraw = 0;
			}

		}
	}
}