#pragma once
#ifndef AUDIOMANAGER_H
#define AUDIOMANAGER_H
#include <iostream>
#include <SDL_mixer.h>

class AudioManager {
public:
	AudioManager();
	~AudioManager();

	bool loadMusic(std::string &path);
	bool loadSound(std::string& path);
	void playMusic(int loops);
	void playSound(int priority);
	void stopMusic();
	void stopSound();
private:
	Mix_Music* music;
	Mix_Chunk* chunk;
};

#endif