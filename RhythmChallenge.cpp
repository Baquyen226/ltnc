#include <SDL.h>
#include <iostream>
#include <algorithm>
#include <cmath>
#include <string>
#include <sstream>
#include <iomanip>

#include "RhythmChallenge.h"
#include "IngameStats.h"
#include "game.h"
#include "constants.h"

//helper functions
int judge(Uint64 offset) {
    double absOffset = (double)offset;
    if (absOffset < 90) {
        std::cerr << "PERFECT\n";
        return 0;
    }
    else if (absOffset < 200) {
        std::cerr << "OK\n";
        return 1;
    }
    else {
        std::cerr << "YOU SUCK\n";
        return 2;
    }
}

void UpdateHealthBar(float* currentHealth, float* targetHealth, float deltaHealth) {
	float tHealth = *targetHealth;
	float cHealth = *currentHealth;

    if (tHealth > 100) {
        tHealth = 100;
    }

    if (tHealth < 0) tHealth = 0;

    if (cHealth > tHealth) {
        cHealth -= deltaHealth * (cHealth - tHealth);
        if (cHealth < tHealth) cHealth = tHealth;
    }
    else if (cHealth < tHealth) {
        cHealth += deltaHealth * (tHealth - cHealth);
        if (cHealth > tHealth) cHealth = tHealth;
    }

	*targetHealth = tHealth;
	*currentHealth = cHealth;
}

std::string RoundToTwoDecimalPlaces(float a) {
    std::ostringstream out;
    a = std::round(a * 100.0f) / 100.0f;
    out << std::fixed << std::setprecision(2) << a;
    return out.str();
}


//class RhythmChallenge
void RhythmChallenge::showJudgement(Game& game, int t) {
    object.active = true;
    object.startTime = SDL_GetTicks();

    switch (t) {
    case 0: // Perfect
        object.textObject = &JUDGEMENT_PERFECT;
        break;
    case 1: // Good
        object.textObject = &JUDGEMENT_GOOD;
        break;
    case 2: // Miss
        object.textObject = &JUDGEMENT_MISS;
        break;
    default:
        object.textObject = &JUDGEMENT_PERFECT;
        break;
    }

	IngameStats* ingameStats = game.getStats();
    if (ingameStats) {
        ingameStats->registerHits(t);  // Update health, combo, and points
        JUDGEMENT_PERFECT.loadTextToTexture("Perfect x" + std::to_string(ingameStats->combo), {255, 255, 255, 255}, game.getRenderer());
        JUDGEMENT_GOOD.loadTextToTexture("Good x" + std::to_string(ingameStats->combo), {255, 255, 255, 255}, game.getRenderer());
		std::string s = RoundToTwoDecimalPlaces(ingameStats->accuracy);
		ACCURACY.loadTextToTexture(s + "%", { 255, 255, 255, 255 }, game.getRenderer());
		ingameStats->Update(-1, NONE, game.getRenderer());  // Update the stats display
    }
}

bool RhythmChallenge::enter(Game& game, SDL_Renderer* renderer) {
    std::cout << "Entering Ingame State with NO MAP DATA(something went wrong)\n";
    //sfx
    harddrop = new AudioManager();
    std::string test = "vine-boom.mp3";
    harddrop->loadSound(test);

    song = new AudioManager();
    test = "bad apple.mp3";
    song->loadMusic(test);

    //beatmap (lazy)
    int BPM = 69;
    int TOTAL_TICKS_PER_MINUTE = 60 * 1000;
    int BEAT_INTERVAL = TOTAL_TICKS_PER_MINUTE / BPM;  // Beat interval in milliseconds
    int OFFSET = BEAT_INTERVAL * 2;

    int song_duration = 216000;  //3:36
    int total_beats = song_duration / BEAT_INTERVAL;

	notes.clear();
    for (int i = 0; i < total_beats; i++) {
        Note newNote;
        newNote.timestamp = OFFSET + BEAT_INTERVAL * i;
        newNote.hit = false;
        newNote.missed = false;
        notes.push_back(newNote);
    }

    //feedback
    JUDGEMENT_PERFECT.loadFont("assets/fonts/8-bit.ttf", 40);
    JUDGEMENT_GOOD.loadFont("assets/fonts/8-bit.ttf", 40);
    JUDGEMENT_MISS.loadFont("assets/fonts/8-bit.ttf", 40);
    HP.loadFont("assets/fonts/8-bit.ttf", 40);
	ACCURACY.loadFont("assets/fonts/8-bit.ttf", 40);

    JUDGEMENT_PERFECT.loadTextToTexture("Perfect", { 255, 255, 255, 255 }, renderer);
    JUDGEMENT_GOOD.loadTextToTexture("Good", { 255, 255, 255, 255 }, renderer);
    JUDGEMENT_MISS.loadTextToTexture("Miss", { 255, 255, 255, 255 }, renderer);
    HP.loadTextToTexture("HP", { 255, 255, 255, 255 }, renderer);
	ACCURACY.loadTextToTexture("0.00%", { 255, 255, 255, 255 }, renderer);

    // Calculate distance from spawn point to hit center
    float hitCenterX = HIT_ZONE_X + 140 / 2;
    spawnDistance = START_X - hitCenterX;  // From right edge to hit center

    //current game properties
    NoteSpeed = 300;
    BEAT_TRAVEL_TIME = spawnDistance / NoteSpeed * 1000;
    gameStart = SDL_GetTicks();

    song->playMusic(0);
    return true;
}

bool RhythmChallenge::enter(Game& game, SDL_Renderer* renderer, MapData* mapData) {
    std::cerr << "Directory name is(this is from RhythmChallenge::enter():" << mapData->directoryName << "\n";
    //sfx
    harddrop = new AudioManager();
    std::string test = "assets/sound/harddrop.wav";
    harddrop->loadSound(test);

    song = new AudioManager();
    test = mapData->directoryName; //line 150
    test.append(mapData->audioFilePath);
    song->loadMusic(test);

    notes.clear();
    for (auto& timestamp : mapData->noteTimestamps) {
        Note newNote;
        newNote.timestamp = timestamp;
        newNote.hit = false;
        newNote.missed = false;
        notes.push_back(newNote);
    }

    //feedback
    JUDGEMENT_PERFECT.loadFont("assets/fonts/8-bit.ttf", 40);
    JUDGEMENT_GOOD.loadFont("assets/fonts/8-bit.ttf", 40);
    JUDGEMENT_MISS.loadFont("assets/fonts/8-bit.ttf", 40);
    HP.loadFont("assets/fonts/8-bit.ttf", 40);
    ACCURACY.loadFont("assets/fonts/8-bit.ttf", 40);

    JUDGEMENT_PERFECT.loadTextToTexture("Perfect", { 255, 255, 255, 255 }, renderer);
    JUDGEMENT_GOOD.loadTextToTexture("Good", { 255, 255, 255, 255 }, renderer);
    JUDGEMENT_MISS.loadTextToTexture("Miss", { 255, 255, 255, 255 }, renderer);
    HP.loadTextToTexture("HP", { 255, 255, 255, 255 }, renderer);
    ACCURACY.loadTextToTexture("0.00%", { 255, 255, 255, 255 }, renderer);

    // Calculate distance from spawn point to hit center
    float hitCenterX = HIT_ZONE_X + 140 / 2;
    spawnDistance = START_X - hitCenterX;  // From right edge to hit center

    //current game properties
    NoteSpeed = 300;
    BEAT_TRAVEL_TIME = spawnDistance / NoteSpeed * 1000;
    gameStart = SDL_GetTicks();

    song->playMusic(0);
    return true;
}

bool RhythmChallenge::exit() {
    std::cout << "Exiting Ingame State\n";
    if (harddrop) delete harddrop;
    harddrop = nullptr;

    if (song) delete song;
    song = nullptr;
    return true;
}

void RhythmChallenge::handleEvent(Game& game, SDL_Event& event) {
    if (ThisIngameState != NORMAL) return;

    switch (event.type) {
    case SDL_EVENT_KEY_DOWN:
        if (event.key.key == SDLK_SPACE && !event.key.repeat) {
            harddrop->playSound(4);
            Uint64 currentTimestamp = SDL_GetTicks() - gameStart;

            // Find the closest note to hit
            Note* closestNote = nullptr;
            Uint64 closestDelta = UINT32_MAX;

            for (auto& note : notes) {
                if (note.hit || note.missed) continue;

                Uint64 delta = SDL_abs(note.timestamp - currentTimestamp);
                if (delta <= HIT_WINDOW && delta < closestDelta) {
                    closestDelta = delta;
                    closestNote = &note;
                }
            }

            // Process the hit for the closest note
            if (closestNote) {
                showJudgement(game, judge(closestDelta));
                closestNote->hit = true;
                closestNote->fadeStartTime = SDL_GetTicks();
            }

            break;
        }
    }
}

void RhythmChallenge::update(Game& game, SDL_Renderer* renderer) {
    Uint64 currentTimestamp = SDL_GetTicks() - gameStart;
    if (game.getStats()->CurrentHealth <= 0.1f) GameOver();

	bool levelComplete = true;

    for (auto& note : notes) {
        if (!note.hit && !note.missed) {
            if (currentTimestamp > note.timestamp + HIT_WINDOW) {
                note.missed = true;
                note.fadeStartTime = SDL_GetTicks();
                showJudgement(game, judge(10000));
            }
			levelComplete = false;
        }
    }

    // Check if current judgment should be deactivated
    if (object.active) {
        Uint64 elapsedTime = SDL_GetTicks() - object.startTime;
        if (elapsedTime > FEEDBACK_DURATION) {
            object.active = false;
        }
    }

	if (levelComplete) ThisIngameState = COMPLETE;
}

void RhythmChallenge::render(Game& game, SDL_Renderer* renderer) {
    Uint64 currentTimestamp = SDL_GetTicks() - gameStart;

    // Render the background bar
    SDL_SetRenderDrawColor(renderer, 172, 93, 132, alpha);
    SDL_FRect bar = { BAR_X, BAR_Y, BAR_WIDTH, BAR_HEIGHT };
    SDL_RenderFillRect(renderer, &bar);

    // Render the hit zone (OK zone)
    SDL_SetRenderDrawColor(renderer, 131, 204, 126, alpha);
    SDL_FRect hitZone = { HIT_ZONE_X, BAR_Y, HIT_ZONE_W, BAR_HEIGHT };
    SDL_RenderFillRect(renderer, &hitZone);

    // Render the perfect hit zone
    SDL_SetRenderDrawColor(renderer, 48, 183, 39, alpha);
    float perfectX = HIT_ZONE_X + (140 - 60) / 2;
    SDL_FRect perfectHitZone = { perfectX, BAR_Y, PERFECT_HIT_ZONE_W, BAR_HEIGHT };
    SDL_RenderFillRect(renderer, &perfectHitZone);

    // Calculate the center point where notes should be perfectly timed
    float hitCenterX = HIT_ZONE_X + HIT_ZONE_W / 2;
    float noteWidth = BAR_WIDTH / 40;

    // Render all notes
    for (auto& note : notes) {
        float noteX = 0.0f;
        float noteY = BAR_Y - 5; // Default Y position slightly above bar
        float this_alpha = 255.0f * alpha / 255;

        if (note.hit || note.missed) {
            // Calculate fade and float effect for hit/missed notes
            float fadeElapsed = (float)(SDL_GetTicks() - note.fadeStartTime);
            float fadeProgress = fadeElapsed / FADE_OUT_TIME;

            // Fade out gradually
            this_alpha = std::max(0.0f, (255.0f - fadeProgress * 255.0f) * alpha / 255);

            // Skip completely faded out notes
            if (this_alpha <= 0) continue;

            if (note.hit) {
                // For hit notes: freeze X position where it was hit and float upward
                // Store the hit position in note.hitPosition if not already set
                if (note.hitPosition == 0) {
                    // Calculate where the note was when hit
                    float timeToHitWhenHit = (float)note.timestamp - (float)(note.fadeStartTime - gameStart);
                    float progressWhenHit = timeToHitWhenHit / BEAT_TRAVEL_TIME;
                    note.hitPosition = hitCenterX + progressWhenHit * spawnDistance;
                }

                noteX = note.hitPosition;

                // Float upward as the note fades out (up to 50 pixels)
                float floatDistance = 50.0f;
                noteY = BAR_Y - 5 - (fadeProgress * floatDistance);
            }
            else {
                // For missed notes: continue normal movement but fade out
                float timeToHit = (float)note.timestamp - (float)currentTimestamp;
                float progress = timeToHit / BEAT_TRAVEL_TIME;
                noteX = hitCenterX + progress * spawnDistance;
            }
        }
        else {
            // Normal note movement for notes that haven't been hit/missed yet
            float timeToHit = (float)note.timestamp - (float)currentTimestamp;
            float progress = timeToHit / BEAT_TRAVEL_TIME;
            noteX = hitCenterX + progress * spawnDistance;

            // Only render active notes that are within visible area
            if (noteX < -noteWidth || noteX > START_X + noteWidth) continue;
        }

        if (ThisIngameState == GAMEOVER) this_alpha = this_alpha * alpha / 255;
        // Draw the note
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, (Uint8)this_alpha);
        SDL_FRect noteRect = {
            noteX - noteWidth / 2,  // Center the note at calculated X position
            noteY,                  // Y position (will float up if hit)
            noteWidth,
            BAR_HEIGHT + 10
        };
        SDL_RenderFillRect(renderer, &noteRect);
    }

    //Render feedback
    if (object.active && object.textObject) {
        Uint64 elapsedTime = SDL_GetTicks() - object.startTime;
        float fadeProgress = (float)elapsedTime / FEEDBACK_DURATION;

        // Calculate the alpha value (fade out as it rises)
        Uint8 alpha = (Uint8)std::max(0.0f, 255.0f * (1.0f - fadeProgress));

        // Calculate y position (float upward)
        float FEEDBACK_FLOAT_DISTANCE = 50.0f;
        float yOffset = FEEDBACK_FLOAT_DISTANCE * fadeProgress;
        float xPos = JUDGEMENT_X;
        float yPos = JUDGEMENT_Y - yOffset;

        // Render the text
        object.textObject->setAlpha(alpha);
        object.textObject->Render(renderer, xPos, yPos);
    }

    //HP bar
    //render healthbar outline
    HP.setAlpha(alpha);
    HP.Render(renderer, HEALTH_BAR_X - 50, HEALTH_BAR_Y - 10);

    SDL_FRect HealthBar = { HEALTH_BAR_X, HEALTH_BAR_Y, HEALTH_BAR_W, HEALTH_BAR_H };
    SDL_SetRenderDrawColor(renderer, 128, 128, 128, alpha);
    SDL_RenderRect(renderer, &HealthBar);
    //Calculate healthbar/maxhealth proportion, then fill the rect according to that
	float* CurrentHealth = &(game.getStats()->CurrentHealth);
	float* ActualHealth = &(game.getStats()->ActualHealth);
    *ActualHealth -= 0.03f;
    /*std::cerr << "Before: " << *CurrentHealth << " " << *ActualHealth << "\n";*/
    UpdateHealthBar(CurrentHealth, ActualHealth, 0.1f);
    /*std::cerr << "After: " << *CurrentHealth << " " << *ActualHealth << "\n";*/

    int w = *CurrentHealth / MaxHealth * HEALTH_BAR_W;
    SDL_FRect HealthLeft = { HEALTH_BAR_X, HEALTH_BAR_Y, w, HEALTH_BAR_H };
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, alpha);
    SDL_RenderFillRect(renderer, &HealthLeft);

	// Render accuracy
	ACCURACY.setAlpha(alpha);
	ACCURACY.Render(renderer, ACCURACY_X, ACCURACY_Y);

    // Reset draw color to black for other rendering
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, alpha);
}

void RhythmChallenge::GameOver() {
    if (ThisIngameState == GAMEOVER) return;

    ThisIngameState = GAMEOVER;
    GameOverTimestamp = SDL_GetTicks();
    Mix_FadeOutMusic(2000);
}

void RhythmChallenge::setAlpha(Uint8 v) {
    alpha = v;
}