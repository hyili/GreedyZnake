#ifndef __SNAKEGAMEAGENT_HEADER__
#include "snakegameagent.h"
#endif

void Snake::reset() {
    _direction = 0;
    _trackingDirection = 0;
    _length = inputLength;
    const std::lock_guard<std::mutex> lock(bodyMutex);
    body.clear();
    for (int i = 0; i < _length; i++) {
        body.push_front(Node(inputY, inputX));
    }
};

Node Snake::getHead() const {
    const std::lock_guard<std::mutex> lock(bodyMutex);
    return body.front();
};

Node Snake::getTail() const {
    const std::lock_guard<std::mutex> lock(bodyMutex);
    return body.back();
};

Nodes Snake::getBodyCopy() const {
    const std::lock_guard<std::mutex> lock(bodyMutex);
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
    const std::lock_guard<std::mutex> lock(bodyMutex);
    body.push_front(Node(y, x));
    body.pop_back();
};

void Snake::grow() {
    _length += 1;
    const std::lock_guard<std::mutex> lock(bodyMutex);
    body.push_front(body.front());
};

void Snake::shrink() {
    if (_length > 1) {
        _length -= 1;
        const std::lock_guard<std::mutex> lock(bodyMutex);
        body.pop_back();
    }
};
