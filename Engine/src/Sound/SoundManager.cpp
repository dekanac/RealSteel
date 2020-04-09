#include "precomp.h"
#include "Sound/SoundManager.h"

#include "Sound/Sound.h"
#include <SDL_mixer.h>

namespace Engine {

	bool SoundManager::AddSound(std::string name_, std::string path_) {

		if (m_Sounds.find(name_) != m_Sounds.end()) {

			LOG_ERROR("Attempt to add add sound twice! name: {}, path {}", name_, path_);
			return false;
		}

		m_Sounds.emplace(name_, std::make_unique<Sound>(path_));

		return m_Sounds.at(name_)->m_Sound != nullptr;
	}
	Sound* SoundManager::GetSound(std::string name_) {

		if (m_Sounds.find(name_) == m_Sounds.end()) {

			LOG_CRITICAL("Attempting to get sound that doesn't exist! name {} ", name_);
			return nullptr;
		}
		
		return m_Sounds.at(name_).get();
	}

	void SoundManager::PlaySound(std::string name_) {

		if (m_Sounds.find(name_) == m_Sounds.end()) {

			LOG_CRITICAL("Attempting to play sound that doesn't exist! name {} ", name_);

		}

		else {

			auto sound = GetSound(name_)->m_Sound;
			
			Mix_PlayChannel(-1, sound, 0);

		}
	}
}