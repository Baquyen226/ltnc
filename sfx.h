#pragma once
#ifndef SFX_H
#define SFX_H

#include <SDL.h>
#include <SDL_mixer.h>

#include "constants.h"

enum PRIORITY {
	GAME_EVENT = 1,
	LINE_CLEAR,
	PIECE_ROTATE,
	PIECE_MOVING
};

class SFX {
public:
	SFX();
	~SFX();
	bool playMusic(std::string &path);
	bool playSound(std::string &path, int priority);
	void stopMusic();
	void stopSound();
private:
	Mix_Music* music;
	Mix_Chunk* sound;
};

extern SFX sfx;


#endif	