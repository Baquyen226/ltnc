#include <iostream>

#include "sfx.h"

SFX::SFX() {
	music = NULL;
	sound = NULL;
}

SFX::~SFX() {
	stopMusic();
	stopSound();
}

bool SFX::playMusic(std::string &path) {
	music = Mix_LoadMUS(path.c_str());
	if (!music) {
		std::cerr << "Failed to load music! SDL_Error: " << SDL_GetError() << std::endl;
		return false;
	}
	else {
		Mix_PlayMusic(music, -1);
	}
	return true;
}

bool SFX::playSound(std::string& path, int priority = 4) {
	sound = Mix_LoadWAV(path.c_str());
	if (!sound) {
		std::cerr << "Failed to load sound! SDL_Error: " << SDL_GetError() << "\n";
		return false;
	}
	else {
		int channel = Mix_PlayChannel(-1, sound, 0);
		if (channel != -1) Mix_GroupChannel(channel, priority);

		if (channel == -1) {
			int oldest = -1;

			for (int i = priority; i <= 4; i++) {
				oldest = Mix_GroupOldest(i);
				//std::cerr << "Checking priority " << i << ", oldest: " << oldest << "\n";
				if (oldest != -1) break;
			}

			if (oldest != -1) {
				//std::cerr << "Halting channel: " << oldest << "\n";
				Mix_HaltChannel(oldest);
				channel = Mix_PlayChannel(-1, sound, 0); // Retry playing sound
				if (channel != -1) Mix_GroupChannel(channel, priority); // Reassign priority
			}
		}

		if (channel == -1) {
			std::cerr << "No available channel for sound: " << path << "\n";
			Mix_FreeChunk(sound);
		}
		//else std::cerr << "Sound " << path << " played on channel " << channel << "\n";
	}
	return true;
}

void SFX::stopMusic() {
	if (music) {
		Mix_FreeMusic(music);
		music = NULL;
	}
}

void SFX::stopSound() {
	if (sound) {
		Mix_FreeChunk(sound);
		sound = NULL;
	}
}

void SFX::setVolume(int v) {
	float _v = MIX_MAX_VOLUME / 100 * (float)v;
	for (int i = 0; i < CHANNEL_COUNT; i++) {
		Mix_Volume(i, _v);
	}
}

SFX sfx;
