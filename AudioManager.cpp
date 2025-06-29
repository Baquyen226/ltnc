#include "AudioManager.h"

AudioManager::AudioManager(){
	music = nullptr;
	chunk = nullptr;
}

AudioManager::~AudioManager() {
	stopMusic();
	stopSound();
}

bool AudioManager::loadMusic(std::string &path){
	music = Mix_LoadMUS(path.c_str());
	if (!music) {
		std::cerr << "Failed to load music! SDL_Error: " << SDL_GetError() << "\n";
		return false;
	}

	return true;
}

bool AudioManager::loadSound(std::string& path) {
	chunk = Mix_LoadWAV(path.c_str());
	if (!chunk) {
		std::cerr << "Failed to load music! SDL_Error: " << SDL_GetError() << "\n";
		return false;
	}

	return true;
}


void AudioManager::playMusic(int loops) {
	if (!music) {
		std::cerr << "Music is not loaded!\n";
		return;
	}
	Mix_PlayMusic(music, loops);
}


void AudioManager::playSound(int priority = 4) {
	if (!chunk) {
		std::cerr << "Failed to load sound! SDL_Error: " << SDL_GetError() << "\n";
		return;
	}
	else {
		int channel = Mix_PlayChannel(-1, chunk, 0);
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
				channel = Mix_PlayChannel(-1, chunk, 0); // Retry playing sound
				if (channel != -1) Mix_GroupChannel(channel, priority); // Reassign priority
			}
		}

		if (channel == -1) {
			std::cerr << "No available channel for sound!\n";
			Mix_FreeChunk(chunk);
		}
		//else std::cerr << "Sound " << path << " played on channel " << channel << "\n";
	}
}

void AudioManager::stopMusic() {
	if (music) {
		Mix_FreeMusic(music);
		music = nullptr;
	}
}

void AudioManager::stopSound() {
	if (chunk) {
		Mix_FreeChunk(chunk);
		chunk = nullptr;
	}
}