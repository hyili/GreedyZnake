#ifndef __SNAKEUTILS_HEADER__
#include "snakeutils.h"
#endif

using namespace std;

const string SnakeGameCounters::getStatusBar() {
    const lock_guard<mutex> lock(gameCountersMutex);
    string statusBar = "fps=" + to_string(fps) + "; aps=" + to_string(aps) +
                       "; tps=" + to_string(tps) + "; round " +
                       to_string(gameNum) + "; s=" + to_string(score) +
                       "; low=" + to_string(lowestScore) +
                       "; high=" + to_string(highestScore) +
                       "; avg=" + to_string(avgScore) + 
                       "; debugMsg=" + debugMsg +
                       ";          ";

    return statusBar;
};

void SnakeGameCounters::updateTPS(int mul) {
    const lock_guard<mutex> lock(gameCountersMutex);
    tps = (int)((double)(trigBot - prevTrigBot) / (double)(timer - prevTimer)) *
          mul;
};

void SnakeGameCounters::increaseTriggerBot() {
    const lock_guard<mutex> lock(gameCountersMutex);
    trigBot += 1;
};

void SnakeGameCounters::setDebugMsg(string s) {
    const lock_guard<mutex> lock(gameCountersMutex);
    debugMsg = s;
};

void SnakeGameCounters::postUpdatePrev() {
    // no need to lock here
    prevTrigBot = trigBot;
};
