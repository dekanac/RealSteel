#include "precomp.h"
#include "Music.h"

#include <SDL_mixer.h>

namespace Engine {

	bool Music::LoadMusic(std::string path_) {

		if (m_Music != nullptr) {

			Mix_FreeMusic(m_Music);
			m_Music = nullptr;
		}
		
		m_Music = Mix_LoadMUS_RW(SDL_RWFromFile(path_.c_str(), "rb"), 1);
		//m_Music = Mix_LoadMUS(path_.c_str());
		
		if (m_Music == nullptr) {

			LOG_CRITICAL("Couldn't load music from file {} ", path_);
		}
		return true;
	}

	Music::Music(std::string path_) {

		LoadMusic(path_);
	}

	Music::~Music() {

		if (m_Music != nullptr) {

			//TODO: izbacuje access violation, popraviti
			//Mix_FreeMusic(m_Music);
			//m_Music = nullptr;
		}
	}
}