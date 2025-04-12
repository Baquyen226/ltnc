#include <SDL_ttf.h>
#include <SDL.h>
#include <string>
#include <iostream>
#include <cmath>

#include "IngameStats.h"
#include "Button.h"

IngameStats::IngameStats() {
	level = 0;
	totalLineCleared = 0;
	lineClearedThisLevel = 0;
	points = 0;
	gravity = BASE_GRAVITY;
	pointsText = new Text;
	pointsText->loadFont("assets/fonts/8-bit.ttf", 24);
	levelText = new Text;
	levelText->loadFont("assets/fonts/8-bit.ttf", 24);
	linesText = new Text;
	linesText->loadFont("assets/fonts/8-bit.ttf", 24);
}

void IngameStats::Reset() {
	level = 0;
	totalLineCleared = 0;
	lineClearedThisLevel = 0;
	points = 0;
	combo = 0;
	totalHits = 0;
	perfectHits = 0;
	goodHits = 0;
	ActualHealth = 100;
	CurrentHealth = 100;
	accuracy = 0.0f;
	gravity = BASE_GRAVITY;
}


void IngameStats::addPoints(int clearedLines) {
	points += line_clear_points_reward[clearedLines - 1] * (level + 1);
	ActualHealth += line_clear_health_reward[clearedLines - 1];
}

void IngameStats::addLines(int lines) {
	totalLineCleared += lines;
	lineClearedThisLevel += lines;
}

void IngameStats::increaseLevel() {
	level++;
	lineClearedThisLevel -= 10;
	gravity = BASE_GRAVITY * pow(0.85, level + 1);
	if (gravity <= 0) gravity = 1;
}

void IngameStats::Update(int clearedLines, SDL_Renderer* renderer) {
	SDL_Color COLOR_WHITE = { 255, 255, 255, alpha };
	if (clearedLines > 0) {
		addPoints(clearedLines);
		addLines(clearedLines);
	}
	//dumbass condition check( i need to update points on hit note :(( )
	if (clearedLines > 0 || !textLoaded || clearedLines == -1) {
		pointsText->loadTextToTexture("Points: " + std::to_string(points), COLOR_WHITE, renderer);
		linesText->loadTextToTexture("Lines: " + std::to_string(totalLineCleared), COLOR_WHITE, renderer);
		levelText->loadTextToTexture("Level: " + std::to_string(level), COLOR_WHITE, renderer);
		textLoaded = true;
	}
}

void IngameStats::Render(SDL_Renderer* renderer) {
	Uint8 r, g, b, a;
	SDL_GetRenderDrawColor(renderer, &r, &g, &b, &a);
	SDL_Color COLOR_WHITE = { 255, 255, 255, alpha };

	//white outline /w black inside(if it works)
	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, alpha);
	SDL_FRect statsRect = { 70, 655, 200, 130 };
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, alpha);
	SDL_RenderFillRect(renderer, &statsRect);
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, alpha);
	SDL_RenderRect(renderer, &statsRect);

	//inside
	
	pointsText->Render(renderer, 100, 670);
	levelText->Render(renderer, 100, 700);
	linesText->Render(renderer, 100, 730);

	SDL_SetRenderDrawColor(renderer, r, g, b, a);
}

int IngameStats::getLevel() {
	return level;
}

int IngameStats::getTotalLinesCleared() {
	return totalLineCleared;
}

int IngameStats::getPoints() {
	return points;
}

void UpdateHealthBar(float& currentHealth, float& targetHealth, float deltaHealth) {
	if (targetHealth > 100.01) {
		targetHealth = 100;
	}

	if (targetHealth < 0) targetHealth = 0;

	if (currentHealth > targetHealth) {
		currentHealth -= deltaHealth * (currentHealth - targetHealth);
		if (currentHealth < targetHealth) currentHealth = targetHealth;
	}
	else if (currentHealth < targetHealth) {
		currentHealth += deltaHealth * (targetHealth - currentHealth);
		if (currentHealth > targetHealth) currentHealth = targetHealth;
	}
}

void IngameStats::registerHits(int judgement) {
	switch (judgement) {
	case 0:
		ActualHealth += 1.0f + 0.01 * combo;
		combo += 1;
		perfectHits += 1;
		totalHits += 1;
		points += 2 * combo;
		break;
	case 1:
		ActualHealth += 0.5f + 0.01 * combo;
		combo += 1;
		goodHits += 1;
		totalHits += 1;
		points += 1 * combo;
		break;
	case 2:
		ActualHealth -= 24.5f;
		combo = 0;
		totalHits += 1;
		break;
	}

	ActualHealth = std::min(100.0f, ActualHealth);
	maxCombo = std::max(combo, maxCombo);
	accuracy = totalHits > 0 ? (float)(perfectHits + goodHits * 0.5f) / totalHits * 100.0f : 0.0f;
}
