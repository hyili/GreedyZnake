#define __UTILS_HEADER__

#include <algorithm>
#include <mutex>
#include <string>

#ifndef __TYPEDEF_HEADER__
#include "typedef.h"
#endif

class GameCounters {
  protected:
    mutable std::mutex gameCountersMutex;

  public:
    long long int frames;
    long long int actions;
    long long int keys;
    long long int timer;
    long long int prevFrames;
    long long int prevActions;
    long long int prevKeys;
    long long int prevTimer;
    int fps;
    int aps;
    std::string debugMsg;

    int gameNum;
    int totalScore;
    int lowestScore;
    int highestScore;
    int score;
    double avgScore;

    GameCounters() {
        frames = actions = keys = timer = prevFrames = prevActions = prevKeys =
            prevTimer = 0;
        fps = 0;
        gameNum = 1;
        lowestScore = highestScore = -1;
        totalScore = score = 0;
        avgScore = 0;
    }
    GameCounters(const GameCounters &ref) {
        // TODO: thread safety
        frames = ref.frames;
        actions = ref.actions;
        keys = ref.keys;
        timer = ref.timer;
        prevFrames = ref.prevFrames;
        prevActions = ref.prevActions;
        prevKeys = ref.prevKeys;
        prevTimer = ref.prevTimer;
        fps = ref.fps;
        aps = ref.aps;

        gameNum = ref.gameNum;
        totalScore = ref.totalScore;
        lowestScore = ref.lowestScore;
        highestScore = ref.highestScore;
        score = ref.score;
        avgScore = ref.avgScore;
    };
    GameCounters(const GameCounters *ref) {
        // TODO: thread safety
        frames = ref->frames;
        actions = ref->actions;
        keys = ref->keys;
        timer = ref->timer;
        prevFrames = ref->prevFrames;
        prevActions = ref->prevActions;
        prevKeys = ref->prevKeys;
        prevTimer = ref->prevTimer;
        fps = ref->fps;
        aps = ref->aps;

        gameNum = ref->gameNum;
        totalScore = ref->totalScore;
        lowestScore = ref->lowestScore;
        highestScore = ref->highestScore;
        score = ref->score;
        avgScore = ref->avgScore;
    };

    void updateFPS(int mul);
    void updateAPS(int mul);
    void updatePrev();
    void updateAvgScore();
    void increaseGameNum();
    void increaseScore();
    void increaseFrame();
    void increaseAction();
    void increaseKey();
    void increaseTimer();
    void updateHighestScore();
    void updateLowestScore();
    void updateTotalScore();

    virtual void postUpdatePrev();
};

