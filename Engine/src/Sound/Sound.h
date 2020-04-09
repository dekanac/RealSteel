#pragma once

struct Mix_Chunk;

namespace Engine {
	
	struct Sound {
		
		Mix_Chunk* m_Sound{};

		bool LoadSound(std::string path_);
		Sound() = default;
		Sound(std::string path_);
		~Sound();

	};
}
