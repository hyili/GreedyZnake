#ifndef __SNAKEGAMEAGENT_HEADER__
#include "snakegameagent.h"
#endif

void Snake::reset() {
    _direction = 0;
    _trackingDirection = 0;
    _length = inputLength;
    const lock_guard<mutex> lock(bodyMutex);
    body.clear();
    for (int i = 0; i < _length; i++) {
        body.push_front(pair<int, int>(inputY, inputX));
    }
};

pair<int, int> Snake::getHead() const {
    const lock_guard<mutex> lock(bodyMutex);
    return body.front();
};

pair<int, int> Snake::getTail() const {
    const lock_guard<mutex> lock(bodyMutex);
    return body.back();
};

list<pair<int, int>> Snake::getBodyCopy() const {
    const lock_guard<mutex> lock(bodyMutex);
    return body;
};

int Snake::getDirection() const { return _direction; };
int Snake::getPrevDirection() const { return _trackingDirection; };

void Snake::updatePrevDirection() { _trackingDirection = _direction; };

int Snake::getLength() const { return _length; };

int Snake::assignDirection(int c) {
    _direction = c;
    return 0;
};

void Snake::updatePosition(int y, int x) {
    updatePrevDirection();
    const lock_guard<mutex> lock(bodyMutex);
    body.push_front(pair<int, int>(y, x));
    body.pop_back();
};

void Snake::grow() {
    _length += 1;
    const lock_guard<mutex> lock(bodyMutex);
    body.push_front(body.front());
};

void Snake::shrink() {
    if (_length > 1) {
        _length -= 1;
        const lock_guard<mutex> lock(bodyMutex);
        body.pop_back();
    }
};
