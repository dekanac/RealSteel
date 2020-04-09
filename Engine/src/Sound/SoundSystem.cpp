#include "precomp.h"
#include "Sound/SoundSystem.h"
#include "Sound/SoundManager.h"
#include "Sound/Sound.h"

#include "ECS/EntityManager.h"

#include <SDL.h>
#include <SDL_mixer.h>

namespace Engine {

	bool SoundSystem::Init() {

		if (SDL_Init(SDL_INIT_AUDIO) < 0) {
			
			LOG_CRITICAL("SDL couldn't init audio!");
			return false;
		}

		if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
			
			LOG_CRITICAL("SDL_mixer could not initialize!");
			return false;
		}

		return true;
	}

	void SoundSystem::Update(float dt, EntityManager* entityManager_) {

	}

	bool SoundSystem::Shutdown() {

		Mix_Quit();
		return true;
	}
}