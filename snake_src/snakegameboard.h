#define __SNAKEGAMEBOARD_HEADER__

#ifndef __GAMEBOARD_HEADER__
#include "gameboard.h"
#endif
#ifndef __SNAKEUTILS_HEADER__
#include "snakeutils.h"
#endif
#ifndef __SNAKEGAMEAGENT_HEADER__
#include "snakegameagent.h"
#endif

class SnakeGameBoard : public GameBoardBase<SnakeGameCounters> {
    Nodes footprint;
    mutable std::mutex footprintMutex;

    std::vector<Snake *> _snakes;
    mutable std::mutex snakesMutex;

  protected:
    int xfood, yfood;
    int footprintLength;
    bool nofood, refresh;

  public:
    SnakeGameBoard(int y, int x, SnakeGameCounters &cnts)
        : GameBoardBase(y, x, cnts) {
        xfood = 0;
        yfood = 0;
        nofood = true;
        refresh = false;
        footprintLength = 10;
    };
    // objects of same class can access each others' private data
    // https://stackoverflow.com/questions/4117002/why-can-i-access-private-variables-in-the-copy-constructor
    SnakeGameBoard(const SnakeGameBoard &ref)
        : GameBoardBase((int)ref.ybound, (int)ref.xbound, NULL) {
        ybound = ref.ybound;
        xbound = ref.xbound;
        movableArea = ref.ybound*ref.xbound;
        yfood = ref.yfood;
        xfood = ref.xfood;
        nofood = ref.nofood;
        refresh = ref.refresh;
        // SnakeGameBoard backgroundMutex would break const qualifier
        setBackground(ref.getBackgroundCopy());
        setFootprint(ref.getFootprintCopy());
        footprintLength = ref.footprintLength;
        isClosed = ref.isClosed;
        isPaused = ref.isPaused;
        _cnts = ref._cnts;
        copySnake(ref);
    };
    SnakeGameBoard(const SnakeGameBoard *ref)
        : GameBoardBase((int)ref->ybound, (int)ref->xbound, NULL) {
        ybound = ref->ybound;
        xbound = ref->xbound;
        movableArea = ref->ybound*ref->xbound;
        yfood = ref->yfood;
        xfood = ref->xfood;
        nofood = ref->nofood;
        refresh = ref->refresh;
        // backgroundMutex and footprintMutex would break const qualifier
        setBackground(ref->getBackgroundCopy());
        setFootprint(ref->getFootprintCopy());
        footprintLength = ref->footprintLength;
        isClosed = ref->isClosed;
        isPaused = ref->isPaused;
        _cnts = ref->_cnts;
        copySnake(*ref);
    };
    ~SnakeGameBoard();

    // prefix const indicates that the function return value is readonly
    // suffix const indicates that the function is readonly
    bool checkRefresh();
    bool checkSnake();
    bool checkFood() const;
    bool checkMovable(const int y, const int x) const;
    bool checkSnakeHead(const int y, const int x) const;
    bool checkSnakeTail(const int y, const int x) const;
    bool checkFence(const int y, const int x) const;
    Snake *getSnake(int index) const;
    int getNumOfSnakes() const;
    Nodes getFootprintCopy() const;
    Node getLatestFootprint() const;
    Node getFood() const;

    const std::vector<std::string> genNextGameBoard() override;
    void pause() override;
    void cont() override;
    void reset() override;
    void close() override;
    int control(int c) override;

    void setRefresh(bool val);
    void placeFoodEvent();
    void moveEvent();
    void eatEvent();
    void updateSnakePosition(int locY, int locX);
    void setFootprint(const Nodes &fp);
    int addSnake(int snakelen, int snakey, int snakex);
    int copySnake(const SnakeGameBoard &ref);
};

