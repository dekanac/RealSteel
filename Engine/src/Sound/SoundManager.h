#pragma once

namespace Engine {

	struct Sound;

	class SoundManager {
	
	public:
		bool AddSound(std::string name_, std::string path_);
		Sound* GetSound(std::string name_);
		void PlaySound(std::string name_);

		SoundManager() = default;

	private:
		std::unordered_map<std::string, std::unique_ptr<Sound>> m_Sounds{};

		SoundManager(const SoundManager& other) = delete;
		SoundManager& operator=(const SoundManager& other) = delete;
	};
}