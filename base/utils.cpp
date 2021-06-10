#ifndef __UTILS_HEADER__
#include "utils.h"
#endif

void GameCounters::updateFPS(int mul) {
    const lock_guard<mutex> lock(gameCountersMutex);
    fps = (int)((double)(frames - prevFrames) / (double)(timer - prevTimer)) *
          mul;
};

void GameCounters::updateAPS(int mul) {
    const lock_guard<mutex> lock(gameCountersMutex);
    aps = (int)((double)(actions - prevActions) / (double)(timer - prevTimer)) *
          mul;
};

void GameCounters::updatePrev() {
    const lock_guard<mutex> lock(gameCountersMutex);
    prevFrames = frames;
    prevActions = actions;
    prevKeys = keys;
    prevTimer = timer;

    postUpdatePrev();
};

void GameCounters::updateAvgScore() {
    const lock_guard<mutex> lock(gameCountersMutex);
    avgScore = (double)totalScore / (double)gameNum;
};

void GameCounters::increaseGameNum() {
    const lock_guard<mutex> lock(gameCountersMutex);
    gameNum += 1;
};

void GameCounters::increaseScore() {
    const lock_guard<mutex> lock(gameCountersMutex);
    score += 1;
};

void GameCounters::increaseFrame() {
    const lock_guard<mutex> lock(gameCountersMutex);
    frames += 1;
};
void GameCounters::increaseAction() {
    const lock_guard<mutex> lock(gameCountersMutex);
    actions += 1;
};
void GameCounters::increaseKey() {
    const lock_guard<mutex> lock(gameCountersMutex);
    keys += 1;
};
void GameCounters::increaseTimer() {
    const lock_guard<mutex> lock(gameCountersMutex);
    timer += 1;
};

void GameCounters::updateLowestScore() {
    const lock_guard<mutex> lock(gameCountersMutex);
    if (lowestScore == -1)
        lowestScore = score;
    lowestScore = min(lowestScore, score);
};

void GameCounters::updateHighestScore() {
    const lock_guard<mutex> lock(gameCountersMutex);
    highestScore = max(highestScore, score);
};

void GameCounters::updateTotalScore() {
    const lock_guard<mutex> lock(gameCountersMutex);
    totalScore += score;
    score = 0;
};

void GameCounters::postUpdatePrev(){
    /* wait for implement */
};
