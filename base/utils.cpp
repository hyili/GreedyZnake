#ifndef __UTILS_HEADER__
#include "utils.h"
#endif

void GameCounters::updateFPS(int mul) {
    const std::lock_guard<std::mutex> lock(gameCountersMutex);
    fps = (int)((double)(frames - prevFrames) / (double)(timer - prevTimer)) *
          mul;
};

void GameCounters::updateAPS(int mul) {
    const std::lock_guard<std::mutex> lock(gameCountersMutex);
    aps = (int)((double)(actions - prevActions) / (double)(timer - prevTimer)) *
          mul;
};

void GameCounters::updatePrev() {
    const std::lock_guard<std::mutex> lock(gameCountersMutex);
    prevFrames = frames;
    prevActions = actions;
    prevKeys = keys;
    prevTimer = timer;

    postUpdatePrev();
};

void GameCounters::updateAvgScore() {
    const std::lock_guard<std::mutex> lock(gameCountersMutex);
    avgScore = (double)totalScore / (double)gameNum;
};

void GameCounters::increaseGameNum() {
    const std::lock_guard<std::mutex> lock(gameCountersMutex);
    gameNum += 1;
};

void GameCounters::increaseScore() {
    const std::lock_guard<std::mutex> lock(gameCountersMutex);
    score += 1;
};

void GameCounters::increaseFrame() {
    const std::lock_guard<std::mutex> lock(gameCountersMutex);
    frames += 1;
};
void GameCounters::increaseAction() {
    const std::lock_guard<std::mutex> lock(gameCountersMutex);
    actions += 1;
};
void GameCounters::increaseKey() {
    const std::lock_guard<std::mutex> lock(gameCountersMutex);
    keys += 1;
};
void GameCounters::increaseTimer() {
    const std::lock_guard<std::mutex> lock(gameCountersMutex);
    timer += 1;
};

void GameCounters::updateLowestScore() {
    const std::lock_guard<std::mutex> lock(gameCountersMutex);
    if (lowestScore == -1)
        lowestScore = score;
    lowestScore = std::min(lowestScore, score);
};

void GameCounters::updateHighestScore() {
    const std::lock_guard<std::mutex> lock(gameCountersMutex);
    highestScore = std::max(highestScore, score);
};

void GameCounters::updateTotalScore() {
    const std::lock_guard<std::mutex> lock(gameCountersMutex);
    totalScore += score;
    score = 0;
};

void GameCounters::postUpdatePrev(){
    /* wait for implement */
};
