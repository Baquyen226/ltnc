#include "AutoplayState.h"
#include "IngameState.h"
#include "game.h"

bool AutoplayState::enter(Game& game, SDL_Renderer* renderer) {
	std::cerr << "Entering Autoplay state\n";
	ingameWrapper = new IngameState;
	ingameWrapper->enter(game, renderer);
	aiEnabled = true; // Enable AI by default
	tetrisAI = ingameWrapper->TetrisPart->getBoard()->getAI();
	game.getStats()->loadAssets(renderer);
	enterTimestamp = SDL_GetTicks();
	notes = ingameWrapper->RhythmPart->getNotes(); // Get notes from the IngameState
	return true;
}

bool AutoplayState::exit() {
	if (ingameWrapper) {
		ingameWrapper->exit();
		delete ingameWrapper;
		ingameWrapper = nullptr;
	}
	return true;
}

void AutoplayState::handleEvent(Game& game, SDL_Event& event) {}

void AutoplayState::update(Game& game, SDL_Renderer* renderer) {
    // Constants
    static constexpr Uint64 PRECISE_HIT_WINDOW = 32;
    static constexpr Uint32 MAX_TIMER = 1000000; // Prevent overflow

    // Initialize AI if needed
    if (!tetrisAI && ingameWrapper->TetrisPart->getBoard()) {
        tetrisAI = ingameWrapper->TetrisPart->getBoard()->getAI();
    }

    // Initialize notes once
    if (notes.empty()) {
        notes = ingameWrapper->RhythmPart->getNotes();
    }

    // Early exit if AI not ready
    if (!(aiEnabled && tetrisAI && ingameWrapper->TetrisPart->getState() == NORMAL)) {
        ingameWrapper->update(game, renderer);
        return;
    }

    // Generate moves if needed
    if (tetrisAI->getMovePlan().empty()) {
        tetrisAI->generateMovePlan(renderer, game);
        if (tetrisAI->getMovePlan().empty()) {
            ingameWrapper->update(game, renderer);
            return; // No moves available
        }
    }

    MovementType move = tetrisAI->getMovePlan().front();

    // Use RhythmChallenge's timing system for consistency
    Uint64 now = SDL_GetTicks() - ingameWrapper->RhythmPart->getGameStart();

    // Clean up missed notes
    for (auto& note : notes) {
        if (!note.hit && !note.missed && note.timestamp + HIT_WINDOW < now) {
            note.missed = true;
        }
    }

    // ---- HARD DROP HANDLING ----
    if (move == MovementType::HARD_DROP) {
        Note* closestNote = nullptr;
        Uint64 closestDelta = UINT64_MAX;

        for (auto& note : notes) {
            if (note.hit || note.missed) continue;
            Uint64 delta = SDL_abs((int64_t)note.timestamp - (int64_t)now);

            if (delta < closestDelta) {
                closestDelta = delta;
                closestNote = &note;
            }
        }

        // Only drop if the closest note is within window
        if (closestNote && closestDelta <= PRECISE_HIT_WINDOW) {
            ingameWrapper->RhythmPart->triggerNearestNote(game);
            ingameWrapper->TetrisPart->getBoard()->movePiece(move);

            // Mark the note as consumed (avoid double-hits)
            closestNote->hit = true;
            closestNote->fadeStartTime = SDL_GetTicks();

            tetrisAI->getMovePlan().pop_front();

            // Skip any queued consecutive hard drops
            while (!tetrisAI->getMovePlan().empty() &&
                tetrisAI->getMovePlan().front() == MovementType::HARD_DROP) {
                tetrisAI->getMovePlan().pop_front();
            }
        }
        // If timing isn’t right, do nothing and wait
    }
    // ---- REGULAR MOVE HANDLING ----
    else {
        if (timer >= AI_MOVE_DELAY) {
            ingameWrapper->TetrisPart->getBoard()->movePiece(move);
            tetrisAI->getMovePlan().pop_front();
            timer = 0;
        }
    }

    // Update timer with overflow protection
    if (timer < MAX_TIMER) {
        timer++;
    }

    // Run wrapper update
    ingameWrapper->update(game, renderer);
}



void AutoplayState::render(Game& game, SDL_Renderer* renderer) {
	if (ingameWrapper) {
		ingameWrapper->render(game, renderer);
	}
}

bool AutoplayState::enter(Game& game, SDL_Renderer* renderer, MapData* mapData) {
	std::cerr << "Entering Autoplay state\n";
	ingameWrapper = new IngameState;
	ingameWrapper->enter(game, renderer, mapData);
	aiEnabled = true; // Enable AI by default
	tetrisAI = ingameWrapper->TetrisPart->getBoard()->getAI();
	notes = ingameWrapper->RhythmPart->getNotes(); // Get notes from the IngameState
	return true;
}