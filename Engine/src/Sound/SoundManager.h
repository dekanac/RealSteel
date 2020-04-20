#pragma once

namespace Engine {

	struct Sound;
	struct Music;

	class SoundManager {
	
	public:
		bool AddSound(std::string name_, std::string path_);
		Sound* GetSound(std::string name_);
		void PlaySound(std::string name_, int loops);
		
		bool AddMusic(std::string name_, std::string path_);
		Music* GetMusic(std::string name_);
		void PlayMusic(std::string name_);
		void StopMusic();
		void PauseMusic();
		void ResumeMusic();


		SoundManager() = default;

	private:
		std::unordered_map<std::string, std::unique_ptr<Sound>> m_Sounds{};
		std::unordered_map<std::string, std::unique_ptr<Music>> m_Music{};

		SoundManager(const SoundManager& other) = delete;
		SoundManager& operator=(const SoundManager& other) = delete;
	};
}