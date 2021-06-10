#define __GAMEROLE_HEADER__

#include <unistd.h>

#include <ncurses.h>

#ifndef __GAMEBOARD_HEADER__
#include "gameboard.h"
#endif

template <typename GB, typename GC> class GameRoleBase {
  protected:
    GB *_gameboard;
    int _id;
    GC *_cnts;

  public:
    GameRoleBase(int id) : _gameboard(NULL), _cnts(NULL) { _id = id; };

    int getID() const;
    const GB *getGameBoard() const;
    void connectGameBoard(GB &gameboard);
    GC *getGameCounters();
    bool checkClosed() const;
    bool checkPaused() const;

    virtual void postConnectGameBoard();
};

// Sol 2. implement in same file
// https://bytefreaks.net/programming-2/c/c-undefined-reference-to-templated-class-function
template <typename GB, typename GC> int GameRoleBase<GB, GC>::getID() const {
    return _id;
};

template <typename GB, typename GC>
const GB *GameRoleBase<GB, GC>::getGameBoard() const {
    return _gameboard;
};

template <typename GB, typename GC>
void GameRoleBase<GB, GC>::connectGameBoard(GB &gameboard) {
    _gameboard = &gameboard;
    _cnts = _gameboard->getGameCounters();

    postConnectGameBoard();
};

template <typename GB, typename GC>
GC *GameRoleBase<GB, GC>::getGameCounters() {
    return _cnts;
};

template <typename GB, typename GC>
bool GameRoleBase<GB, GC>::checkClosed() const {
    return _gameboard->checkClosed();
};

template <typename GB, typename GC>
bool GameRoleBase<GB, GC>::checkPaused() const {
    return _gameboard->checkPaused();
};

template <typename GB, typename GC>
void GameRoleBase<GB, GC>::postConnectGameBoard(){/* Do nothing */};
