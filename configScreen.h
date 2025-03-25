#pragma once
#ifndef CONFIGSCREEN_H
#define CONFIGSCREEN_H

#include <fstream>
#include <sstream>

#include "UI.h"

class ConfigScreen {
public:
	Button BACK; //This will get reworked
	Button SAVE;

	ConfigScreen();
	void handleEvents(SDL_Event& event);
	void Render(SDL_Renderer* renderer);
	void loadConfig();
	void saveConfig();
private:
	Slider DAS;	Text DAS_T;					//Slowest : 300ms, Fastest: 1ms	
	Slider RepeatRate; Text RepeatRate_T;	//Slowest : 300ms, Fastest: 1ms
	Slider Volume;	Text Volume_T;			//Loudest : 100,	Min : 0
};

#endif