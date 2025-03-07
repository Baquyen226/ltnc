#include <ctime>
#include <random>
#include <iostream>
#include <vector>
#include <cstdlib>

#include "queue.h"
#include "constants.h"

void Queue::generateNewBag(){
    std::random_device rd;
    std::mt19937 g(rd());

    std::shuffle(bag.begin(), bag.end(), g);
    for (int i : bag) {
        pieceQueue.push_back(i);
    }
}

void Queue::previewQueueUpdate() {
    previewQueue.clear();

    while (pieceQueue.size() < PIECE_PREVIEW) {
        generateNewBag();
    }

    for (int i = 0; i < PIECE_PREVIEW; i++) {
        if (pieceQueue.empty()) generateNewBag();
        previewQueue.push_back(pieceQueue.at(i));
    }

    pieceQueue.erase(pieceQueue.begin());
}

int Queue::getNextPiece() {
    if (pieceQueue.empty() && previewQueue.empty()) {
        std::cerr << "Cannot find next piece";
        exit(-1);
    }

    int nextPiece = previewQueue.at(0);
    previewQueueUpdate();
    return nextPiece;
}
