#ifndef __SNAKEUTILS_HEADER__
#include "snakeutils.h"
#endif

const std::string SnakeGameCounters::getStatusBar() {
    const std::lock_guard<std::mutex> lock(gameCountersMutex);
    std::string statusBar = "fps=" + std::to_string(fps) + "; aps=" + std::to_string(aps) +
                       "; tps=" + std::to_string(tps) + "; round " +
                       std::to_string(gameNum) + "; s=" + std::to_string(score) +
                       "; low=" + std::to_string(lowestScore) +
                       "; high=" + std::to_string(highestScore) +
                       "; avg=" + std::to_string(avgScore) + 
                       "; debugMsg=" + debugMsg +
                       ";          ";

    return statusBar;
};

void SnakeGameCounters::updateTPS(int mul) {
    const std::lock_guard<std::mutex> lock(gameCountersMutex);
    tps = (int)((double)(trigBot - prevTrigBot) / (double)(timer - prevTimer)) *
          mul;
};

void SnakeGameCounters::increaseTriggerBot() {
    const std::lock_guard<std::mutex> lock(gameCountersMutex);
    trigBot += 1;
};

void SnakeGameCounters::setDebugMsg(std::string s) {
    const std::lock_guard<std::mutex> lock(gameCountersMutex);
    debugMsg = s;
};

void SnakeGameCounters::postUpdatePrev() {
    // no need to lock here
    prevTrigBot = trigBot;
};
