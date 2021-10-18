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
        Node loc;
        double cost;

        AStarNode(const Node &l, double c) {
            loc = l;
            cost = c;
        };
    };

    class AStarComp {
      public:
        bool operator()(const AStarNode &a, const AStarNode &b) const;
    };

    typedef std::priority_queue<AStarNode, std::vector<AStarNode>, AStarComp> AStarCostPQ;
    typedef std::vector<double> AStarCostRow;
    typedef std::vector<AStarCostRow> AStarCostMap;
    typedef std::vector<int> FillVisitedRow;
    typedef std::vector<FillVisitedRow> FillVisitedMap;
    typedef std::vector<int> HMTPathRow;
    typedef std::vector<HMTPathRow> HMTPathMap;

    void setDebugMsg();
    int teleportY(int y) const;
    int teleportX(int x) const;
    bool checkHeadOrMovable(const Node &curr) const;
    bool checkHeadOrMovable(int y, int x) const;
    bool checkMovable(const Node &curr) const;
    bool checkMovable(int y, int x) const;
    bool checkFence(const Node &curr) const;
    bool checkFence(int y, int x) const;
    double heuristic(const Node &curr,
                     const Node &end) const;
    Node AStarFindNextNodeFromNeighbor(const Node curr,
                                                 int target) const;
    void AStarMarkBestPath(Node &start, Node &end);
    void AStarProcess(Node curr, Node &end, int distance);
    int AStarGetAction(const Node &curr) const;
    void AStarReset();
    void AStarReset(int y, int x);
    int AStar(Node start, Node end);

    int Wander(Node &start, int prevDirection);

    int FillGetAction(Node &start);
    void FillReset();
    int FillCheckInside(Node curr, int &rounds,
                        Node &target);
    int FillProcess(Node &start, Node &target, int rounds);
    std::vector<Node> FillFindNeighbor(Node curr, int status);
    int Fill(Node &start);

    /* Hamiltonian Path */
    int HMTGetAction(Node &curr, Node &target);
    void HMTInit();
    void HMTReset();
    bool HMTProcess(Node curr, int index);
    int HMT(Node &start, Node &end);


    bool AStarPreservePath;
    AStarCostPQ pq;
    AStarCostMap map;
    FillVisitedMap visited;
    HMTPathMap hmt;
    int mapSize;
    int prevDirection;
    int AStarCounter, WanderCounter, HMTCounter;
    std::string debugMsg;

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
