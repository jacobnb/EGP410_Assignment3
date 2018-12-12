#pragma once
#include <Trackable.h>
#include "../../External Libs/SDL2_mixer-2.0.1/include/SDL_mixer.h"

//based on http://lazyfoo.net/SDL_tutorials/lesson11/index.php

class SoundManager : public Trackable
{
public:
	SoundManager();
	~SoundManager();
	bool init();
	void cleanup();
	bool loadFiles();
	void playMusic();
	void stopMusic();
private:
	Mix_Music * mpMusic;
	Mix_Chunk * mpTakingDamage;
	char *musicFileName = "sound/East_Texas_Pines.mp3";
	char *sfxFileName;
};