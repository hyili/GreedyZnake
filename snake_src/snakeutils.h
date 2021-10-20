#define __SNAKEUTILS_HEADER__

#include <algorithm>
#include <string>

#ifndef __UTILS_HEADER__
#include "utils.h"
#endif

class SnakeGameCounters : public GameCounters {
    long long int trigBot;
    long long int prevTrigBot;
    int tps;

  public:
    SnakeGameCounters() : GameCounters() {
        trigBot = 0;
        prevTrigBot = 0;
        tps = 0;
    };
    SnakeGameCounters(const SnakeGameCounters &ref) : GameCounters(ref) {
        trigBot = ref.trigBot;
        prevTrigBot = ref.prevTrigBot;
        tps = ref.tps;
    };
    SnakeGameCounters(const SnakeGameCounters *ref) : GameCounters(ref) {
        trigBot = ref->trigBot;
        prevTrigBot = ref->prevTrigBot;
        tps = ref->tps;
    };

    const std::string getStatusBar();
    void updateTPS(int mul);
    void increaseTriggerBot();
    void setDebugMsg(std::string s);

    void postUpdatePrev() override;
};
