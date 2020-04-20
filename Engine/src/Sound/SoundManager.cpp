#include "precomp.h"
#include "Sound/SoundManager.h"

#include "Sound/Sound.h"
#include "Sound/Music.h"
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

	void SoundManager::PlaySound(std::string name_, int loops) {

		if (m_Sounds.find(name_) == m_Sounds.end()) {

			LOG_CRITICAL("Attempting to play sound that doesn't exist! name {} ", name_);

		}

		else {

			auto sound = GetSound(name_)->m_Sound;
			
			Mix_PlayChannel(-1, sound, loops);

		}
	}


	bool SoundManager::AddMusic(std::string name_, std::string path_) {

		if (m_Music.find(name_) != m_Music.end()) {

			LOG_ERROR("Attempt to add add music twice! name: {}, path {}", name_, path_);
			return false;
		}

		m_Music.emplace(name_, std::make_unique<Music>(path_));

		return m_Music.at(name_)->m_Music != nullptr;
	}

	Music* SoundManager::GetMusic(std::string name_) {

		if (m_Music.find(name_) == m_Music.end()) {

			LOG_CRITICAL("Attempting to get music that doesn't exist! name {} ", name_);
			return nullptr;
		}

		return m_Music.at(name_).get();
	}

	void SoundManager::PlayMusic(std::string name_) {

		if (m_Music.find(name_) == m_Music.end()) {

			LOG_CRITICAL("Attempting to play music that doesn't exist! name: {} ", name_);

		}

		else {

			auto music = GetMusic(name_)->m_Music;

			Mix_PlayMusic(music, -1);

		}
	}

	void SoundManager::StopMusic() {
		
		if (Mix_PlayingMusic() == 1) {
		
			Mix_HaltMusic();
		}
	}
	void SoundManager::PauseMusic() {
		
		if (Mix_PlayingMusic() == 1) {

			Mix_PauseMusic();
		}
	}
	void SoundManager::ResumeMusic() {
		
		if (Mix_PausedMusic() == 1) {

			Mix_ResumeMusic();
		}
	}
}