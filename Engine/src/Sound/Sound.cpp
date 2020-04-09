#include "precomp.h"
#include "Sound.h"

#include <SDL_mixer.h>

namespace Engine {

	bool Sound::LoadSound(std::string path_) {

		if (m_Sound != nullptr) {

			Mix_FreeChunk(m_Sound);
			m_Sound = nullptr;
		}

		m_Sound = Mix_LoadWAV_RW(SDL_RWFromFile(path_.c_str(), "rb"), 1);
		
		if (m_Sound == nullptr) {
			
			LOG_CRITICAL("Couldn't load sound from file {} ", path_);
		}
		return true;
	}

	Sound::Sound(std::string path_) {
		
		LoadSound(path_);
	}

	Sound::~Sound() {

		if (m_Sound != nullptr) {

			Mix_FreeChunk(m_Sound);
		}
	}
}