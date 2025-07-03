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
	Reset();
}

void IngameStats::loadAssets(SDL_Renderer* renderer) {
	pointsText = new Text;
	pointsText->loadFont("assets/fonts/8-bit.ttf", 24);
	levelText = new Text;
	levelText->loadFont("assets/fonts/8-bit.ttf", 24);
	linesText = new Text;
	linesText->loadFont("assets/fonts/8-bit.ttf", 24);
	B2BText = new Text;
	B2BText->loadFont("assets/fonts/8-bit.ttf", 24);

	// init clears and spins texts
	CLEAR_SINGLE.loadFont("assets/fonts/8-bit.ttf", 24);
	CLEAR_DOUBLE.loadFont("assets/fonts/8-bit.ttf", 24);
	CLEAR_TRIPLE.loadFont("assets/fonts/8-bit.ttf", 24);
	CLEAR_QUAD.loadFont("assets/fonts/8-bit.ttf", 24);
	CLEAR_SINGLE.loadTextToTexture("SINGLE", { 255, 255, 255, 255 }, renderer);
	CLEAR_DOUBLE.loadTextToTexture("DOUBLE", { 255, 255, 255, 255 }, renderer);
	CLEAR_TRIPLE.loadTextToTexture("TRIPLE", { 255, 255, 255, 255 }, renderer);
	CLEAR_QUAD.loadTextToTexture("QUAD", { 255, 255, 255, 255 }, renderer);

	NOTHINGLOL.loadFont("assets/fonts/8-bit.ttf", 24);
	NOTHINGLOL.loadTextToTexture(" ", { 255, 255, 255, 255 }, renderer);

	T_SPIN.loadFont("assets/fonts/8-bit.ttf", 16);
	T_SPIN_MINI.loadFont("assets/fonts/8-bit.ttf", 16);
	T_SPIN.loadTextToTexture("T-SPIN", { 255, 255, 255, 255 }, renderer);
	T_SPIN_MINI.loadTextToTexture("MINI T-SPIN", { 255, 255, 255, 255 }, renderer);
	textLoaded = true;
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


void IngameStats::addPoints(int clearedLines, TSpinType type) {
	//a bit dangerous since tspin arrays are not the same size as line clear arrays
	double multiplier = (b2b > 0) ? 1.5 : 1;
	switch (type) {
	case TSpinType::NONE:
		points += line_clear_points_reward[clearedLines] * (level + 1) * multiplier;
		ActualHealth += line_clear_health_reward[clearedLines];
		if (clearedLines == 4) b2b++;
		else b2b = -1;
		break;
	case TSpinType::T_SPIN:
		points += TSpin_points_reward[clearedLines] * (level + 1) * multiplier;
		ActualHealth += TSpin_health_reward[clearedLines];
		if (clearedLines >= 1) b2b++;
		break;
	case TSpinType::T_SPIN_MINI:
		points += TSpinMini_points_reward[clearedLines] * (level + 1) * multiplier;
		ActualHealth += TSpinMini_health_reward[clearedLines];
		if (clearedLines >= 1) b2b++;
		break;
	}
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

void IngameStats::Update(int clearedLines, TSpinType type, SDL_Renderer* renderer) {
	SDL_Color COLOR_WHITE = { 255, 255, 255, alpha };

	if (clearedLines > 0 || type != NONE) {
		addPoints(clearedLines, type);
		addLines(clearedLines);
		B2BActive = (b2b > 0);

		getLineClearText(clearedLines, type);
		clearActive = true;
		lineClearStartTime = SDL_GetTicks();
		//std::cerr << "[IngameStats] Update called with" << clearedLines << " lines, and " << type << " T-spin!\n";
	}

	points = std::ceil(points);

	//theres a clearedLines == -1 specifically reserved for every time a note is hit(maybe i shouldnt have done that)
	if (clearedLines > 0 || !textLoaded || clearedLines == -1) {
		pointsText->loadTextToTexture("Points: " + std::to_string((int)points), COLOR_WHITE, renderer);
		linesText->loadTextToTexture("Lines: " + std::to_string(totalLineCleared), COLOR_WHITE, renderer);
		levelText->loadTextToTexture("Level: " + std::to_string(level), COLOR_WHITE, renderer);
		B2BText->loadTextToTexture("Back-to-Back x" + std::to_string(b2b), COLOR_WHITE, renderer);
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
	
	pointsText->Render(renderer, 100, 670);
	levelText->Render(renderer, 100, 700);
	linesText->Render(renderer, 100, 730);

	if (clearActive) {
		Uint64 currentTime = SDL_GetTicks();
		if (currentTime - lineClearStartTime < LINE_CLEAR_TEXT_LIFE_TIME) {
			double alpha = 255.0 * (LINE_CLEAR_TEXT_LIFE_TIME - (currentTime - lineClearStartTime)) / LINE_CLEAR_TEXT_LIFE_TIME;
			lineClearText->setAlpha(alpha);
			TSpinText->setAlpha(alpha);
			int x, y;
			lineClearText->getSize(&x, &y);
			int lineClearTextX = BOARD_OFFSET_X - LINE_CLEAR_TEXT_X_OFFSET_FROM_BOARD - x;
			//std::cerr << "[IngameStats] Rendering line clear text at " << lineClearTextX << ", " << LINE_CLEAR_TEXT_Y << "\n";
			lineClearText->Render(renderer, lineClearTextX , LINE_CLEAR_TEXT_Y);
			
			TSpinText->getSize(&x, &y);
			int TSpinTextX = BOARD_OFFSET_X - TSPIN_TEXT_X_OFFSET_FROM_BOARD - x;
			//std::cerr << "[IngameStats] Rendering T-spin text at " << TSpinTextX << ", " << TSPIN_TEXT_Y << "\n";
			TSpinText->Render(renderer, TSpinTextX, TSPIN_TEXT_Y);
		}
		else clearActive = false;
	}

	// Render B2B text if active
	if (B2BActive) {
		B2BText->setAlpha(alpha);
		int x, y;
		B2BText->getSize(&x, &y);
		int B2BTextX = BOARD_OFFSET_X - TSPIN_TEXT_X_OFFSET_FROM_BOARD - x;
		B2BText->Render(renderer, B2BTextX, TSPIN_TEXT_Y + 100);
	}

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

void IngameStats::getLineClearText(int lines, TSpinType type) {
	switch (lines) {
	case 1:
		lineClearText = &CLEAR_SINGLE;
		break;
	case 2:
		lineClearText = &CLEAR_DOUBLE;
		break;
	case 3:
		lineClearText = &CLEAR_TRIPLE;
		break;
	case 4:
		lineClearText = &CLEAR_QUAD;
		break;
	default:
		lineClearText = &NOTHINGLOL;
		break;
	}

	switch (type) {
	case TSpinType::T_SPIN:
		TSpinText = &T_SPIN;
		break;
	case TSpinType::T_SPIN_MINI:
		TSpinText = &T_SPIN_MINI;
		break;
	default:
		TSpinText = &NOTHINGLOL;
		break;
	}
	std::cerr << "[IngameStats] Line clear text set to " << lineClearText->getText() << " and T-spin text set to " << TSpinText->getText() << "\n";
}
