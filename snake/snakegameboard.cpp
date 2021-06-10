#include <ncurses.h>

#ifndef __SNAKEGAMEBOARD_HEADER__
#include "snakegameboard.h"
#endif

bool SnakeGameBoard::checkRefresh() {
    if (refresh)
        return true;
    return false;
};

bool SnakeGameBoard::checkSnake() {
    // TODO: currently not support multiple snakes
    if (getNumOfSnakes() != 0)
        return true;
    return false;
};

bool SnakeGameBoard::checkFood() const { return !nofood; };

bool SnakeGameBoard::checkMovable(const int y, const int x) const {
    if (checkValid(y, x) && (getYX(y, x) == ' ' || getYX(y, x) == '@'))
        return true;
    return false;
}

bool SnakeGameBoard::checkSnakeHead(const int y, const int x) const {
    if (checkValid(y, x) && getYX(y, x) == '%')
        return true;
    return false;
}

bool SnakeGameBoard::checkSnakeTail(const int y, const int x) const {
    if (checkValid(y, x) && getYX(y, x) == '~')
        return true;
    return false;
}

bool SnakeGameBoard::checkFence(const int y, const int x) const {
    if (checkValid(y, x) && (getYX(y, x) == '-' || getYX(y, x) == '|'))
        return true;
    return false;
}

Snake *SnakeGameBoard::getSnake(int index) const {
    // TODO: currently not support multiple snakes
    const lock_guard<mutex> lock(snakesMutex);
    return _snakes[index];
};

int SnakeGameBoard::getNumOfSnakes() const {
    const lock_guard<mutex> lock(snakesMutex);
    return _snakes.size();
};

list<pair<int, int>> SnakeGameBoard::getFootprintCopy() const {
    const lock_guard<mutex> lock(footprintMutex);
    return footprint;
};

pair<int, int> SnakeGameBoard::getLatestFootprint() const {
    const lock_guard<mutex> lock(footprintMutex);
    for (auto ptr = footprint.begin(); ptr != footprint.end(); ptr++) {
        if (checkMovable(ptr->first, ptr->second)) return *ptr;
    }
    return footprint.back();
};

pair<int, int> SnakeGameBoard::getFood() const {
    return pair<int, int>(yfood, xfood);
};

const vector<string> SnakeGameBoard::genNextGameBoard() {
    vector<string> tempGameBoard;

    // draw background
    const vector<string> background = getBackgroundCopy();
    for (int i = 0; i < background.size(); i++) {
        tempGameBoard.push_back(background[i]);
    }

    // draw snake
    // TODO: currently not support multiple snakes
    if (checkSnake()) {
        // draw snake body
        list<pair<int, int>> body = getSnake(0)->getBodyCopy();
        for (auto ptr = body.begin(); ptr != body.end(); ptr++) {
            tempGameBoard[ptr->first][ptr->second] = '*';
        }

        pair<int, int> head(getSnake(0)->getHead());
        pair<int, int> tail(getSnake(0)->getTail());

        // draw snake tail
        tempGameBoard[tail.first][tail.second] = '~';

        // draw snake head
        tempGameBoard[head.first][head.second] = '%';

    }

    // draw food
    if (checkFood())
        tempGameBoard[yfood][xfood] = '@';

    return tempGameBoard;
};

SnakeGameBoard::~SnakeGameBoard() {
    // TODO: currently not support multiple snakes
    for (int index = 0; index < getNumOfSnakes(); index++)
        delete getSnake(index);
};

void SnakeGameBoard::cont() { isPaused = false; }

void SnakeGameBoard::pause() { isPaused = true; }

void SnakeGameBoard::reset() {
    // TODO: currently not support multiple snakes
    if (checkSnake())
        getSnake(0)->reset();

    _cnts->updateLowestScore();
    _cnts->updateHighestScore();
    _cnts->updateTotalScore();
    _cnts->updateAvgScore();
    _cnts->increaseGameNum();

    xfood = 0;
    yfood = 0;
    isClosed = false;
    nofood = true;
    updateGameBoard();
};

void SnakeGameBoard::close() {
    isClosed = true;
    while (getNumOfSnakes() > 0) {
        const lock_guard<mutex> lock(snakesMutex);
        Snake *snake = _snakes.back();
        delete snake;
        _snakes.pop_back();
    }
};

int SnakeGameBoard::control(int c) {
    // TODO: currently not support multiple snakes
    if (checkSnake()) {
        int prevDirection = getSnake(0)->getPrevDirection();
        switch (c) {
        case 'g':
            getSnake(0)->grow();
            break;
        case 'l':
            if (!checkRefresh())
                setRefresh(true);
            break;
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
        case 's':
            getSnake(0)->shrink();
            break;
        case KEY_UP:
            if (prevDirection == KEY_DOWN)
                break;
            getSnake(0)->assignDirection(c);
            break;
        case KEY_DOWN:
            if (prevDirection == KEY_UP)
                break;
            getSnake(0)->assignDirection(c);
            break;
        case KEY_LEFT:
            if (prevDirection == KEY_RIGHT)
                break;
            getSnake(0)->assignDirection(c);
            break;
        case KEY_RIGHT:
            if (prevDirection == KEY_LEFT)
                break;
            getSnake(0)->assignDirection(c);
            break;
        }
    } else {
        switch (c) {
        case 'q':
            close();
            break;
        case 'r':
            reset();
            break;
        }
    }
    return 0;
};

void SnakeGameBoard::setRefresh(bool val) { refresh = val; };

void SnakeGameBoard::placeFoodEvent() {
    if (checkFood())
        return;

    int ybound = getYBound();
    int xbound = getXBound();

    // find a place for food
    int length = getSnake(0)->getLength();
    int target = rand() % max((xbound * ybound - length), 1), counter = 0;
    const vector<string> currGameBoard = getCurrGameBoardCopy();
    while (true) {
        for (int i = 0; i < xbound; i++) {
            for (int j = 0; j < ybound; j++) {
                if (currGameBoard[j][i] == ' ' && (counter++ == target)) {
                    xfood = i;
                    yfood = j;
                    nofood = false;
                    return;
                }
            }
        }
    }
};

void SnakeGameBoard::moveEvent() {
    // TODO: currently not support multiple snakes
    if (checkSnake()) {
        pair<int, int> loc = getSnake(0)->getHead();
        int locY, locX;
        switch (getSnake(0)->getDirection()) {
        case KEY_UP:
            if (checkMovable(locY = teleportY(loc.first - 1),
                             locX = teleportX(loc.second))) {
                updateSnakePosition(locY, locX);
            }
            break;
        case KEY_DOWN:
            if (checkMovable(locY = teleportY(loc.first + 1),
                             locX = teleportX(loc.second))) {
                updateSnakePosition(locY, locX);
            }
            break;
        case KEY_LEFT:
            if (checkMovable(locY = teleportY(loc.first),
                             locX = teleportX(loc.second - 1))) {
                updateSnakePosition(locY, locX);
            }
            break;
        case KEY_RIGHT:
            if (checkMovable(locY = teleportY(loc.first),
                             locX = teleportX(loc.second + 1))) {
                updateSnakePosition(locY, locX);
            }
            break;
        }
    }
};

void SnakeGameBoard::eatEvent() {
    // TODO: currently not support multiple snakes
    if (checkSnake()) {
        pair<int, int> loc = getSnake(0)->getHead();
        if (loc.first == yfood && loc.second == xfood) {
            getSnake(0)->grow();
            _cnts->increaseScore();
            nofood = true;

            // no food now, update and place a new one
            updateGameBoard();
            placeFoodEvent();
        }
    }
};

void SnakeGameBoard::updateSnakePosition(int locY, int locX) {
    // TODO: currently not support multiple snakes
    const lock_guard<mutex> lock(footprintMutex);
    if (footprint.size() > footprintLength)
        footprint.pop_back();
    footprint.push_front(getSnake(0)->getTail());
    getSnake(0)->updatePosition(locY, locX);
};

void SnakeGameBoard::setFootprint(const list<pair<int, int>> &fp) {
    const lock_guard<mutex> lock(footprintMutex);
    footprint = fp;
};

int SnakeGameBoard::addSnake(int snakelen, int snakey, int snakex) {
    // TODO: currently not support multiple snakes
    if (getNumOfSnakes() >= 1)
        return -1;

    if (!checkMovable(teleportY(snakey), teleportX(snakex)))
        return -1;

    const lock_guard<mutex> lock(snakesMutex);
    _snakes.push_back(
        new Snake(snakelen, teleportY(snakey), teleportX(snakex)));

    return 0;
};

int SnakeGameBoard::copySnake(const SnakeGameBoard &ref) {
    for (int index = 0; index < ref.getNumOfSnakes(); index++) {
        const lock_guard<mutex> lock(snakesMutex);
        _snakes.push_back(new Snake(ref.getSnake(index)));
    }

    return 0;
};
