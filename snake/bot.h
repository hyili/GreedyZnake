#define __BOT_HEADER__
#include <fstream>
#include <iostream>
#include <queue>
#include <stack>
#include <unistd.h>

#include <ncurses.h>

#ifndef __SNAKEGAMEROLE_HEADER__
#include "snakegamerole.h"
#endif

#ifndef __SNAKEGAMEBOARD_HEADER__
#include "snakegameboard.h"
#endif

class Bot {
    SnakeGamePlayer *_gameplayer;
    char method;

    class AStarNode {
      public:
        pair<int, int> loc;
        double cost;

        AStarNode(const pair<int, int> &l, double c) {
            loc = l;
            cost = c;
        };
    };

    class AStarComp {
      public:
        bool operator()(const AStarNode &a, const AStarNode &b) const;
    };

    typedef priority_queue<AStarNode, vector<AStarNode>, AStarComp> AStarCostPQ;
    typedef vector<double> AStarCostRow;
    typedef vector<AStarCostRow> AStarCostMap;
    typedef vector<int> FillVisitedRow;
    typedef vector<FillVisitedRow> FillVisitedMap;
    typedef vector<int> HMTPathRow;
    typedef vector<HMTPathRow> HMTPathMap;

    void setDebugMsg();
    int teleportY(int y) const;
    int teleportX(int x) const;
    bool checkHeadOrMovable(const pair<int, int> &curr) const;
    bool checkHeadOrMovable(int y, int x) const;
    bool checkMovable(const pair<int, int> &curr) const;
    bool checkMovable(int y, int x) const;
    bool checkFence(const pair<int, int> &curr) const;
    bool checkFence(int y, int x) const;
    double heuristic(const pair<int, int> &curr,
                     const pair<int, int> &end) const;
    pair<int, int> AStarFindNextSpotFromNeighbor(const pair<int, int> curr,
                                                 int target) const;
    void AStarMarkBestPath(pair<int, int> &start, pair<int, int> &end);
    void AStarProcess(pair<int, int> curr, pair<int, int> &end, int distance);
    int AStarGetAction(const pair<int, int> &curr) const;
    void AStarReset();
    void AStarReset(int y, int x);
    int AStar(pair<int, int> start, pair<int, int> end);

    int Wander(pair<int, int> &start, int prevDirection);

    int FillGetAction(pair<int, int> &start);
    void FillReset();
    int FillCheckInside(pair<int, int> curr, int &rounds,
                        pair<int, int> &target);
    int FillProcess(pair<int, int> &start, pair<int, int> &target, int rounds);
    vector<pair<int, int>> FillFindNeighbor(pair<int, int> curr, int status);
    int Fill(pair<int, int> &start);

    /* Hamiltonian Path */
    int HMTGetAction(pair<int, int> &curr, pair<int, int> &target);
    void HMTInit();
    void HMTReset();
    bool HMTProcess(pair<int,int> curr, int index);
    int HMT(pair<int,int> &start, pair<int, int> &end);


    bool AStarPreservePath;
    AStarCostPQ pq;
    AStarCostMap map;
    FillVisitedMap visited;
    HMTPathMap hmt;
    int mapSize;
    int prevDirection;
    int AStarCounter, WanderCounter, HMTCounter;
    string debugMsg;

  public:
    Bot(SnakeGamePlayer &gameplayer, char m) {
        _gameplayer = &gameplayer;
        AStarPreservePath = false;
        map = AStarCostMap(_gameplayer->getYBound(),
                           AStarCostRow(_gameplayer->getXBound(), -1));
        visited = FillVisitedMap(_gameplayer->getYBound(),
                                 FillVisitedRow(_gameplayer->getXBound(), 0));
        hmt = HMTPathMap(_gameplayer->getYBound(), HMTPathRow(_gameplayer->getXBound(), -1));
        mapSize = _gameplayer->getYBound() * _gameplayer->getXBound();
        AStarCounter = 0;
        WanderCounter = 0;
        HMTCounter = 0;
        prevDirection = KEY_RIGHT;
        method = m;
    };
    int generate();
    void close();
};
