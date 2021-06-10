#ifndef __SNAKEGAMEROLE_HEADER__
#include "snakegamerole.h"
#endif

vector<string> SnakeGamePlayer::getCurrGameBoardCopy() const {
    return _gameboard->getCurrGameBoardCopy();
};

bool SnakeGamePlayer::checkSnake() const { return _gameboard->checkSnake(); };

bool SnakeGamePlayer::checkFood() const { return _gameboard->checkFood(); };

bool SnakeGamePlayer::checkValid(int y, int x) const {
    return _gameboard->checkValid(y, x);
};

bool SnakeGamePlayer::checkMovable(const int y, const int x) const {
    return _gameboard->checkMovable(y, x);
};

bool SnakeGamePlayer::checkSnakeHead(const int y, const int x) const {
    return _gameboard->checkSnakeHead(y, x);
};

bool SnakeGamePlayer::checkSnakeTail(const int y, const int x) const {
    return _gameboard->checkSnakeTail(y, x);
};

bool SnakeGamePlayer::checkFence(const int y, const int x) const {
    return _gameboard->checkFence(y, x);
};

bool SnakeGamePlayer::checkBackground() const {
    return _gameboard->checkBackground();
};

vector<string> SnakeGamePlayer::getBackgroundCopy() const {
    return _gameboard->getBackgroundCopy();
};

const Snake *SnakeGamePlayer::getSnake(int index) const {
    return _gameboard->getSnake(index);
};

pair<int, int> SnakeGamePlayer::getLatestFootprint() const {
    return _gameboard->getLatestFootprint();
};

int SnakeGamePlayer::getXBound() const { return _gameboard->getXBound(); };

int SnakeGamePlayer::getYBound() const { return _gameboard->getYBound(); };

int SnakeGamePlayer::getMovableArea() const { return _gameboard->getMovableArea(); };

int SnakeGamePlayer::teleportY(int y) const {
    return _gameboard->teleportY(y);
};

int SnakeGamePlayer::teleportX(int x) const {
    return _gameboard->teleportX(x);
};

pair<int, int> SnakeGamePlayer::getFood() const {
    return _gameboard->getFood();
};

int SnakeGamePlayer::control(int c) { return _gameboard->control(c); };

void SnakeGamePlayer::setDebugMsg(string s) { _cnts->setDebugMsg(s); };

SnakeGameMaster &SnakeGamePlayer::getFullAccess() {
    SnakeGameMaster *target = dynamic_cast<SnakeGameMaster *>(this);

    return *target;
};

/*
void SnakeGameMaster::placeFoodEvent() { _gameboard->placeFoodEvent(); };

void SnakeGameMaster::moveEvent() { _gameboard->moveEvent(); };

void SnakeGameMaster::eatEvent() { _gameboard->eatEvent(); };

void SnakeGameMaster::updateSnakePosition(int locY, int locX) {
    _gameboard->updateSnakePosition(locY, locX);
};
*/

void SnakeGameCamera::postConnectGameBoard() {
    preparedScreen =
        vector<string>(toScreenY(_gameboard->getYBound()),
                       string(toScreenX(_gameboard->getXBound()), ' '));
    currScreen =
        vector<string>(toScreenY(_gameboard->getYBound()),
                       string(toScreenX(_gameboard->getXBound()), ' '));
    prevScreen =
        vector<string>(toScreenY(_gameboard->getYBound()),
                       string(toScreenX(_gameboard->getXBound()), ' '));
    updatePreparedScreen();
};

void SnakeGameCamera::updatePreparedScreen() {
    const vector<string> currGameBoard = _gameboard->getCurrGameBoardCopy();

    // conversion
    for (int y = 0; y < _gameboard->getYBound(); y++) {
        for (int x = 0; x < _gameboard->getXBound(); x++) {
            preparedScreen[toScreenY(y)][toScreenX(x)] = currGameBoard[y][x];
        }
    }
};

bool SnakeGameCamera::checkRefreshSchedule() {
    return _gameboard->checkRefresh();
};

void SnakeGameCamera::scheduleRefresh() { _gameboard->setRefresh(true); };

void SnakeGameCamera::cancelRefresh() { _gameboard->setRefresh(false); };

void SnakeGameCamera::print() {
    int x, y;
    string statusBar = _cnts->getStatusBar();

    // reset
    prevScreen = currScreen;

    // build one line screen
    currScreen = preparedScreen;

    // update different
    if (checkRefreshSchedule()) {
        for (y = 0; y < currScreen.size(); y++) {
            for (x = 0; x < currScreen[y].size(); x++) {
                ::move(y, x);
                ::addch(currScreen[y][x]);
            }
        }
        cancelRefresh();
    } else {
        for (y = 0; y < currScreen.size(); y++) {
            for (x = 0; x < currScreen[y].size(); x++) {
                if (currScreen[y][x] != prevScreen[y][x]) {
                    ::move(y, x);
                    ::addch(currScreen[y][x]);
                }
            }
        }
    }

    ::move(toScreenY(_gameboard->getYBound()), 0);
    addstr(statusBar.c_str());

    // DEPRECATED
    // addstr(currOneLineScreen.c_str());
};

int SnakeGameCamera::toScreenY(int y) { return y; };

int SnakeGameCamera::toScreenX(int x) { return x << 1; };

int SnakeGameCamera::toLogicalY(int y) { return y; };

int SnakeGameCamera::toLogicalX(int x) { return x >> 1; };
