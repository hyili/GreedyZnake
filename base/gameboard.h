#define __GAMEBOARD_HEADER__

#include <climits>
#include <cstdlib>
#include <list>
#include <mutex>
#include <random>
#include <string>
#include <vector>

#ifndef __GAMEAGENT_HEADER__
#include "gameagent.h"
#endif
#ifndef __UTILS_HEADER__
#include "utils.h"
#endif

using namespace std;

template <typename GC> class GameBoardBase {
    vector<string> currGameBoard;
    mutable mutex currGameBoardMutex;

    vector<string> background;
    mutable mutex backgroundMutex;

    void applyNextGameBoard(const vector<string> &nextGameBoard);

  protected:
    int xbound, ybound;
    int movableArea;
    GC *_cnts;
    bool isClosed, isPaused;

  public:
    GameBoardBase(int y, int x, GC *cnts) {
        ybound = y;
        xbound = x;
        movableArea = y*x;
        _cnts = cnts;
        isClosed = isPaused = false;
    };
    GameBoardBase(int y, int x, GC &cnts) {
        ybound = y;
        xbound = x;
        movableArea = y*x;
        _cnts = &cnts;
        isClosed = isPaused = false;
    };

    vector<string> getCurrGameBoardCopy() const;
    bool checkClosed() const;
    bool checkPaused() const;
    bool checkValid(int y, int x) const;
    bool checkBackground() const;
    GC *getGameCounters() const;
    vector<string> getBackgroundCopy() const;
    char getYX(int y, int x) const;
    int getXBound() const;
    int getYBound() const;
    int getMovableArea() const;
    int teleportY(int y) const;
    int teleportX(int x) const;

    void updateGameBoard();

    virtual const vector<string> genNextGameBoard();
    virtual int setBackground(bool fence);
    virtual int setBackground(const vector<string> &ref);
    virtual void pause();
    virtual void cont();
    virtual void reset();
    virtual void close();
    virtual int control(int c);

    void setGameCounters(GC &cnts);
};

template <typename GC>
vector<string> GameBoardBase<GC>::getCurrGameBoardCopy() const {
    const lock_guard<mutex> lock(currGameBoardMutex);
    return currGameBoard;
};

template <typename GC> bool GameBoardBase<GC>::checkClosed() const {
    return isClosed;
};

template <typename GC> bool GameBoardBase<GC>::checkPaused() const {
    return isPaused;
};

template <typename GC> bool GameBoardBase<GC>::checkValid(int y, int x) const {
    if (y >= 0 && y < ybound && x >= 0 && x < xbound)
        return true;
    return false;
}

template <typename GC> bool GameBoardBase<GC>::checkBackground() const {
    const lock_guard<mutex> lock(backgroundMutex);
    return background.size() == ybound && ybound > 0 &&
           background[0].size() == xbound;
};

template <typename GC> GC *GameBoardBase<GC>::getGameCounters() const {
    return _cnts;
};

template <typename GC>
vector<string> GameBoardBase<GC>::getBackgroundCopy() const {
    const lock_guard<mutex> lock(backgroundMutex);
    return background;
};

template <typename GC> char GameBoardBase<GC>::getYX(int y, int x) const {
    const lock_guard<mutex> lock(currGameBoardMutex);
    return currGameBoard[y][x];
};

template <typename GC> int GameBoardBase<GC>::getXBound() const {
    return xbound;
};

template <typename GC> int GameBoardBase<GC>::getYBound() const {
    return ybound;
};

template <typename GC> int GameBoardBase<GC>::getMovableArea() const {
    return movableArea;
};

template <typename GC> int GameBoardBase<GC>::teleportY(int y) const {
    int ans = y % ybound;
    return ans < 0 ? ans + ybound : ans;
}

template <typename GC> int GameBoardBase<GC>::teleportX(int x) const {
    int ans = x % xbound;
    return ans < 0 ? ans + xbound : ans;
}

template <typename GC> void GameBoardBase<GC>::updateGameBoard() {
    if (checkBackground()) {
        applyNextGameBoard(genNextGameBoard());
    }
};

template <typename GC>
const vector<string> GameBoardBase<GC>::genNextGameBoard() {
    vector<string> tempGameBoard;

    // draw background
    const lock_guard<mutex> lock(backgroundMutex);
    for (int i = 0; i < background.size(); i++) {
        tempGameBoard.push_back(background[i]);
    }

    // draw any other things here

    return tempGameBoard;
};

template <typename GC>
void GameBoardBase<GC>::applyNextGameBoard(
    const vector<string> &nextGameBoard) {
    const lock_guard<mutex> lock(currGameBoardMutex);
    currGameBoard = nextGameBoard;
};

template <typename GC> int GameBoardBase<GC>::setBackground(bool fence) {
    if (!checkBackground()) {
        const lock_guard<mutex> lock(backgroundMutex);
        background = vector<string>(ybound, string(xbound, ' '));
        if (fence) {
            for (int i = 0; i < xbound; i++) {
                background[0][i] = '-';
                background[ybound - 1][i] = '-';
            }
            for (int i = 0; i < ybound; i++) {
                background[i][0] = '|';
                background[i][xbound - 1] = '|';
            }
            movableArea = (ybound-2) * (xbound-2);
        }
    }
    updateGameBoard();

    return 0;
};

template <typename GC>
int GameBoardBase<GC>::setBackground(const vector<string> &ref) {
    if (!checkBackground()) {
        const lock_guard<mutex> lock(backgroundMutex);
        background = ref;
    }
    updateGameBoard();

    return 0;
}

template <typename GC> void GameBoardBase<GC>::cont() { isPaused = false; }

template <typename GC> void GameBoardBase<GC>::pause() { isPaused = true; }

template <typename GC> void GameBoardBase<GC>::reset() {
    _cnts->updateLowestScore();
    _cnts->updateHighestScore();
    _cnts->updateTotalScore();
    _cnts->updateAvgScore();
    _cnts->increaseGameNum();

    isClosed = false;
    updateGameBoard();
};

template <typename GC> void GameBoardBase<GC>::close() { isClosed = true; };

template <typename GC> int GameBoardBase<GC>::control(int c) {
    switch (c) {
    case 'p':
        if (checkPaused())
            cont();
        else
            pause();
        break;
    case 'q':
        pause();
        close();
        cont();
        break;
    case 'r':
        pause();
        reset();
        cont();
        break;
    }

    return 0;
};

template <typename GC> void GameBoardBase<GC>::setGameCounters(GC &cnts) {
    _cnts = &cnts;
};
