#define __SNAKEGAMEROLE_HEADER__

#ifndef __GAMEROLE_HEADER__
#include "gamerole.h"
#endif
#ifndef __SNAKEGAMEBOARD_HEADER__
#include "snakegameboard.h"
#endif
#ifndef __SNAKEGAMEAGENT_HEADER__
#include "snakegameagent.h"
#endif

class SnakeGameMaster;

/* member functions for bot development */
class SnakeGamePlayer : public GameRoleBase<SnakeGameBoard, SnakeGameCounters> {
  public:
    SnakeGamePlayer(int id)
        : GameRoleBase<SnakeGameBoard, SnakeGameCounters>(id){};
    // objects of same class can access each others' private data
    // https://stackoverflow.com/questions/4117002/why-can-i-access-private-variables-in-the-copy-constructor
    SnakeGamePlayer(const SnakeGamePlayer &ref)
        : GameRoleBase<SnakeGameBoard, SnakeGameCounters>((int)ref._id) {
        _gameboard = ref._gameboard;
        _cnts = ref._cnts;
    };
    SnakeGamePlayer(const SnakeGamePlayer *ref)
        : GameRoleBase<SnakeGameBoard, SnakeGameCounters>((int)ref->_id) {
        _gameboard = ref->_gameboard;
        _cnts = ref->_cnts;
    };

    std::vector<std::string> getCurrGameBoardCopy() const;
    bool checkSnake() const;
    bool checkFood() const;
    bool checkValid(int y, int x) const;
    bool checkMovable(const int y, const int x) const;
    bool checkSnakeHead(const int y, const int x) const;
    bool checkSnakeTail(const int y, const int x) const;
    bool checkFence(const int y, const int x) const;
    bool checkBackground() const;
    std::vector<std::string> getBackgroundCopy() const;
    const Snake *getSnake(int index) const;
    Node getLatestFootprint() const;
    int getXBound() const;
    int getYBound() const;
    int getMovableArea() const;
    int teleportY(int y) const;
    int teleportX(int x) const;
    Node getFood() const;

    int control(int c);
    void setDebugMsg(std::string s);

    SnakeGameMaster &getFullAccess();
};

class SnakeGameMaster : public SnakeGamePlayer {

  public:
    SnakeGameMaster(int id) : SnakeGamePlayer(id){};

    /*
    void placeFoodEvent();
    void moveEvent();
    void eatEvent();
    void updateSnakePosition(int locY, int locX);
    */
};

class SnakeGameCamera : public GameRoleBase<SnakeGameBoard, SnakeGameCounters> {
    std::vector<std::string> preparedScreen;
    std::vector<std::string> currScreen;
    std::vector<std::string> prevScreen;
    std::string prevOneLineScreen;
    std::string currOneLineScreen;

    void postConnectGameBoard() override;

    int toScreenY(int y);
    int toScreenX(int x);
    int toLogicalY(int y);
    int toLogicalX(int x);

  public:
    SnakeGameCamera(int id)
        : GameRoleBase<SnakeGameBoard, SnakeGameCounters>(id){};

    ~SnakeGameCamera(){};

    void updatePreparedScreen();
    bool checkRefreshSchedule();
    void scheduleRefresh();
    void cancelRefresh();
    void print();
};
