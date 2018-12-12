#include "SoundManager.h"

SoundManager::SoundManager()
{
}

SoundManager::~SoundManager()
{
}

bool SoundManager::init()
{
	if (SDL_Init(SDL_INIT_AUDIO) == -1) {
		std::cout << "error initing sdl audio\n";
		return false;
	}
	int flags = MIX_INIT_MP3;
	int initted = Mix_Init(flags);
	if (initted&flags != flags) {
		std::cout << "Mix_Init: Failed to init required ogg and mod support!\n";
		printf("Mix_Init: %s\n", Mix_GetError());
		return false;
	}
	if (Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096) == -1)
	{
		std::cout << "Mix_Init: Failed to open audio\n";
		return false;
	}
	return true;
}

void SoundManager::cleanup()
{
	Mix_Quit();
}

bool SoundManager::loadFiles()
{
	mpMusic = Mix_LoadMUS(musicFileName);
	return mpMusic != NULL;
}

void SoundManager::playMusic()
{
	if (Mix_PlayingMusic() == 0) {
		if (Mix_PlayMusic(mpMusic, -1) == -1) {
			std::cout << "Error playing music\n";
		}
	}
}

void SoundManager::stopMusic()
{
	Mix_HaltMusic();
}
