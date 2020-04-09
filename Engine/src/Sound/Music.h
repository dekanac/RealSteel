#pragma once

#include <SDL_mixer.h>

namespace Engine {

	struct Music {

		Mix_Music* m_Music{};

		bool LoadMusic(std::string path_);
		Music() = default;
		Music(std::string path_);
		~Music();

	};
}
