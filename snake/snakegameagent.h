#define __SNAKEGAMEAGENT_HEADER__

#ifndef __GAMEAGENT_HEADER__
#include "gameagent.h"
#endif

class Snake : public GameAgentBase {
    Nodes body;
    mutable std::mutex bodyMutex;

    int _length;
    int _direction;
    int _trackingDirection;
    int inputLength, inputX, inputY;

    void updatePrevDirection();

  public:
    Snake(int len, int y, int x) : GameAgentBase() {
        _direction = 0;
        _trackingDirection = 0;
        _length = inputLength = std::max(1, len);
        inputY = y;
        inputX = x;
        for (int i = 0; i < _length; i++) {
            const std::lock_guard<std::mutex> lock(bodyMutex);
            body.push_front(Node(inputY, inputX));
        }
    };
    Snake(const Snake &ref) : GameAgentBase() {
        body = ref.getBodyCopy();
        _length = ref._length;
        _direction = ref._direction;
        _trackingDirection = ref._trackingDirection;
        inputLength = ref.inputLength;
        inputX = ref.inputX;
        inputY = ref.inputY;
    }
    Snake(const Snake *ref) : GameAgentBase() {
        body = ref->getBodyCopy();
        _length = ref->_length;
        _direction = ref->_direction;
        _trackingDirection = ref->_trackingDirection;
        inputLength = ref->inputLength;
        inputX = ref->inputX;
        inputY = ref->inputY;
    }

    // suffix const indicates that the function is readonly
    virtual void reset() override;
    Node getHead() const;
    Node getTail() const;
    Nodes getBodyCopy() const;
    int getDirection() const;
    int getPrevDirection() const;
    int getLength() const;

    int assignDirection(int c);
    void updatePosition(int y, int x);
    void grow();
    void shrink();
};

