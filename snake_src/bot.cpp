#ifndef __BOT_HEADER__
#include "bot.h"
#endif

/* strategy:
 * 1. find the path from head to food using A* algorithm
 * 2. if step 1. cannot find path, find the path from head to tail using A*
 * algorithm
 * 3. step 1. 2. can both be improved: always choose a path besides the fence or
 * body
 * 4. Other improvements
 */

void Bot::setDebugMsg() { _gameplayer->setDebugMsg(debugMsg); }

int Bot::teleportY(int y) const { return _gameplayer->teleportY(y); }

int Bot::teleportX(int x) const { return _gameplayer->teleportX(x); }

bool Bot::checkHeadOrMovable(const Node &curr) const {
    return _gameplayer->checkMovable(curr.first, curr.second) ||
           _gameplayer->checkSnakeHead(curr.first, curr.second);
}

bool Bot::checkHeadOrMovable(int y, int x) const {
    return _gameplayer->checkMovable(y, x) || _gameplayer->checkSnakeHead(y, x);
}

bool Bot::checkMovable(const Node &curr) const {
    return _gameplayer->checkMovable(curr.first, curr.second);
}

bool Bot::checkMovable(int y, int x) const {
    return _gameplayer->checkMovable(y, x);
}

bool Bot::checkFence(const Node &curr) const {
    return _gameplayer->checkFence(curr.first, curr.second);
}

bool Bot::checkFence(int y, int x) const {
    return _gameplayer->checkFence(y, x);
}

/*
 * max-pq, when a < b return true
 * min-pq, when a > b return true
 */
bool Bot::AStarComp::operator()(const AStarNode &a, const AStarNode &b) const {
    if (a.cost > b.cost)
        return true;
    return false;
}

double Bot::heuristic(const Node &curr,
                      const Node &end) const {
    double yOffset = std::min(teleportY(end.first - curr.first),
                         teleportY(curr.first - end.first));
    double xOffset = std::min(teleportX(end.second - curr.second),
                         teleportX(curr.second - end.second));

    return sqrt(yOffset * yOffset + xOffset * xOffset);
}

Node Bot::AStarFindNextNodeFromNeighbor(const Node curr,
                                                  int target) const {
    int locY, locX;

    locY = teleportY(curr.first - 1);
    locX = curr.second;
    if (map[locY][locX] == target)
        return Node(locY, locX);

    locY = teleportY(curr.first + 1);
    if (map[locY][locX] == target)
        return Node(locY, locX);

    locY = curr.first;
    locX = teleportX(curr.second - 1);
    if (map[locY][locX] == target)
        return Node(locY, locX);

    locX = teleportX(curr.second + 1);
    if (map[locY][locX] == target)
        return Node(locY, locX);

    return Node(-1, -1);
}

void Bot::AStarMarkBestPath(Node &start, Node &end) {
    int sy = start.first, sx = start.second, ey = end.first, ex = end.second;
    int y = sy, x = sx, target;
    Node next;

    while (!(y == ey && x == ex)) {
        if ((y == -1) && (x == -1)) {
            AStarReset();
            return;
        }

        target = map[y][x] - 1;
        map[y][x] = -map[y][x] - 2;
        next = AStarFindNextNodeFromNeighbor(Node(y, x), target);
        y = next.first;
        x = next.second;
    }

    target = map[y][x] - 1;
    map[y][x] = -map[y][x] - 2;
}

void Bot::AStarProcess(Node curr, Node &end, int distance) {
    if (checkHeadOrMovable(curr) && (map[curr.first][curr.second] == -1)) {
        double cost = distance + heuristic(curr, end);
        AStarNode currAStarNode = AStarNode(curr, cost);
        map[curr.first][curr.second] = distance;
        pq.push(currAStarNode);
    }

    return;
}

int Bot::AStarGetAction(const Node &curr) const {
    int temp;

    if (map[curr.first][curr.second] != -1) {
        Node next = AStarFindNextNodeFromNeighbor(
            curr, map[curr.first][curr.second] - 1);

        if (next.first == -1 || next.second == -1)
            return 0;

        if ((temp = teleportX(next.second - curr.second)) == 1)
            return KEY_RIGHT;
        if (temp == teleportX(-1))
            return KEY_LEFT;
        if ((temp = teleportY(next.first - curr.first)) == 1)
            return KEY_DOWN;
        if (temp == teleportY(-1))
            return KEY_UP;
    }

    return 0;
}

void Bot::AStarReset() {
    while (!pq.empty()) {
        pq.pop();
    }
    int ybound = _gameplayer->getYBound();
    int xbound = _gameplayer->getXBound();

	if (AStarPreservePath) {
		for (int y = 0; y < ybound; y++) {
            for (int x = 0; x < xbound; x++) {
                if (map[y][x] >= -1) map[y][x] = -1;
                else map[y][x] = INT_MAX;
            }
        }
	} else {
		for (int y = 0; y < ybound; y++) {
            for (int x = 0; x < xbound; x++) {
                map[y][x] = -1;
            }
        }
    }
}

void Bot::AStarReset(int y, int x) {
    if (!_gameplayer->checkValid(y, x)) return;

	if (AStarPreservePath) {
        if (map[y][x] >= -1) map[y][x] = -1;
        else map[y][x] = INT_MAX;
	} else {
        map[y][x] = -1;
    }
}

int Bot::AStar(Node start, Node end) {
    int result = 0;

    // TODO: REMOVE
//    if (start.first == 9 && start.second == 82) {
//        debugMsg = to_std::string(start.first) + "_" + to_std::string(start.second) + "_" + to_std::string(end.first) + "_" + to_std::string(end.second);
//        setDebugMsg();
//    }

//    if (start.first == 9 && start.second == 82) {
//        debugMsg = to_std::string(map[9][82]);
//        setDebugMsg();
//    }

    // initialize the starting set
    int distance = 0;
    AStarProcess(start, end, distance);

    while (true) {
        // we can't find path
        if (pq.empty()) {
            AStarReset();
            break;
        }

        AStarNode currAStarNode = pq.top();
        pq.pop();
        distance = map[currAStarNode.loc.first][currAStarNode.loc.second];

        // we got end here
        if ((currAStarNode.loc.first == end.first) &&
            (currAStarNode.loc.second == end.second)) {
            // from end to start, not from start to end
            AStarMarkBestPath(end, start);
            break;
        }

        // update the neighbors
        AStarProcess(Node(teleportY(currAStarNode.loc.first - 1),
                                    currAStarNode.loc.second),
                     end, distance + 1);
        AStarProcess(Node(teleportY(currAStarNode.loc.first + 1),
                                    currAStarNode.loc.second),
                     end, distance + 1);
        AStarProcess(Node(currAStarNode.loc.first,
                                    teleportX(currAStarNode.loc.second - 1)),
                     end, distance + 1);
        AStarProcess(Node(currAStarNode.loc.first,
                                    teleportX(currAStarNode.loc.second + 1)),
                     end, distance + 1);
    }

    // return 0 if we cannot find path
    result = AStarGetAction(start);
    return result;
}

int Bot::Wander(Node &start, int prevDirection) {
    std::vector<int> actions;
    if (checkMovable(teleportY(start.first - 1), start.second)) {
        if (KEY_UP == prevDirection)
            return prevDirection;
        actions.push_back(KEY_UP);
    }
    if (checkMovable(teleportY(start.first + 1), start.second)) {
        if (KEY_DOWN == prevDirection)
            return prevDirection;
        actions.push_back(KEY_DOWN);
    }
    if (checkMovable(start.first, teleportX(start.second - 1))) {
        if (KEY_LEFT == prevDirection)
            return prevDirection;
        actions.push_back(KEY_LEFT);
    }
    if (checkMovable(start.first, teleportX(start.second + 1))) {
        if (KEY_RIGHT == prevDirection)
            return prevDirection;
        actions.push_back(KEY_RIGHT);
    }

    if (actions.empty())
        return 0;

    int index = rand() % actions.size();
    return actions[index];
}

int Bot::FillGetAction(Node &start) { return 0; }

void Bot::FillReset() {
    int ybound = _gameplayer->getYBound();
    int xbound = _gameplayer->getXBound();
    for (int y = 0; y < ybound; y++) {
        for (int x = 0; x < xbound; x++) {
            if (checkMovable(y, x))
                visited[y][x] = 0;
            else
                visited[y][x] = INT_MIN;
        }
    }

    const Snake *snake = _gameplayer->getSnake(0);
    Nodes body = snake->getBodyCopy();
    int val = 1;

    for (auto ritr = body.crbegin(); ritr != body.crend(); ritr++)
        visited[ritr->first][ritr->second] = val++;

    AStarReset();
}

/* Perform DFS to:
 * find the weakest coordinate to break through (y,x)(target),
 * find the minimum round we need before arriving the weakest coordinate
 * (rounds).
 * find the remaining spaces inside the area (spaces).
 * return the number of spaces.
 *
 * TODO:
 * if visited map records 0 means not visited.
 * if visited map records -1 means visited.
 * if visited map records INT_MAX means there is a fence.
 * if visited map records val > 0 means there is a snake body.
 * */
int Bot::FillCheckInside(Node curr, int &rounds,
                         Node &target) {
    auto ptr = visited[curr.first].begin() + curr.second;
    int spaces = 0;

    if (*ptr == -1) return spaces;

    /* when ptr is snake body and it will disappear first compare to the previous one. */
    /* set ptr as our temporary target */
    if (*ptr > 0 && rounds > *ptr) {
        rounds = *ptr;
        target = curr;
        return spaces;
    }

    /* if ptr is not visted */
    if (*ptr == 0) {
        // set to visited
        *ptr = -1;
        spaces += 1;

        spaces += FillCheckInside(
            Node(teleportY(curr.first - 1), curr.second), rounds,
            target);
        spaces += FillCheckInside(
            Node(teleportY(curr.first + 1), curr.second), rounds,
            target);
        spaces += FillCheckInside(
            Node(curr.first, teleportX(curr.second - 1)), rounds,
            target);
        spaces += FillCheckInside(
            Node(curr.first, teleportX(curr.second + 1)), rounds,
            target);
    }

    return spaces;
}

/*
 * Fill the spaces using right-hand rule
 * and return the next direction of start
 */
int Bot::FillProcess(Node &start, Node &target, int rounds) {
    int currDirection = 0, nextDirection = 0;
    bool extended = false;

    // TODO: can still be improved
    AStarReset();
    currDirection = AStar(start, target);
    Node next = AStarFindNextNodeFromNeighbor(start, map[start.first][start.second] - 1);
    if (next.first != -1 && next.second != -1)
        nextDirection = AStarGetAction(next);

    switch(currDirection) {
        /* check LEFT then RIGHT */
        case KEY_UP:
            if (nextDirection != KEY_LEFT && checkMovable(start.first, teleportX(start.second-1)) &&
                    checkMovable(teleportY(start.first-1), teleportX(start.second-1))) {
                currDirection = KEY_LEFT;
                extended = true;
                if (checkMovable(teleportY(start.first+1), start.second) &&
                        checkMovable(teleportY(start.first+1), teleportX(start.second-1))) {
                    currDirection = KEY_DOWN;
                }
            }
            if (extended) break;
            if (nextDirection != KEY_RIGHT && checkMovable(start.first, teleportX(start.second+1)) &&
                    checkMovable(teleportY(start.first-1), teleportX(start.second+1))) {
                currDirection = KEY_RIGHT;
                if (checkMovable(teleportY(start.first+1), start.second) &&
                        checkMovable(teleportY(start.first+1), teleportX(start.second+1))) {
                    currDirection = KEY_DOWN;
                }
            }
            break;
        /* check LEFT then RIGHT */
        case KEY_DOWN:
            if (nextDirection != KEY_LEFT && checkMovable(start.first, teleportX(start.second-1)) &&
                    checkMovable(teleportY(start.first+1), teleportX(start.second-1))) {
                currDirection = KEY_LEFT;
                extended = true;
                if (checkMovable(teleportY(start.first-1), start.second) &&
                        checkMovable(teleportY(start.first-1), teleportX(start.second-1))) {
                    currDirection = KEY_UP;
                }
            }
            if (extended) break;
            if (nextDirection != KEY_RIGHT && checkMovable(start.first, teleportX(start.second+1)) &&
                    checkMovable(teleportY(start.first+1), teleportX(start.second+1))) {
                currDirection = KEY_RIGHT;
                if (checkMovable(teleportY(start.first-1), start.second) &&
                        checkMovable(teleportY(start.first-1), teleportX(start.second+1))) {
                    currDirection = KEY_UP;
                }
            }
            break;
        /* check UP then DOWN */
        case KEY_LEFT:
            if (nextDirection != KEY_UP && checkMovable(teleportY(start.first-1), start.second) &&
                    checkMovable(teleportY(start.first-1), teleportX(start.second-1))) {
                currDirection = KEY_UP;
                extended = true;
                if (checkMovable(start.first, teleportX(start.second+1)) &&
                        checkMovable(teleportY(start.first-1), teleportX(start.second+1))) {
                    currDirection = KEY_RIGHT;
                }
            }
            if (extended) break;
            if (nextDirection != KEY_DOWN && checkMovable(teleportY(start.first+1), start.second) &&
                    checkMovable(teleportY(start.first+1), teleportX(start.second-1))) {
                currDirection = KEY_DOWN;
                if (checkMovable(start.first, teleportX(start.second+1)) &&
                        checkMovable(teleportY(start.first+1), teleportX(start.second+1))) {
                    currDirection = KEY_RIGHT;
                }
            }
            break;
        /* check UP then DOWN */
        case KEY_RIGHT:
            if (nextDirection != KEY_UP && checkMovable(teleportY(start.first-1), start.second) &&
                    checkMovable(teleportY(start.first-1), teleportX(start.second+1))) {
                currDirection = KEY_UP;
                extended = true;
                if (checkMovable(start.first, teleportX(start.second-1)) &&
                        checkMovable(teleportY(start.first-1), teleportX(start.second-1))) {
                    currDirection = KEY_LEFT;
                }
            }
            if (extended) break;
            if (nextDirection != KEY_DOWN && checkMovable(teleportY(start.first+1), start.second) &&
                    checkMovable(teleportY(start.first+1), teleportX(start.second+1))) {
                currDirection = KEY_DOWN;
                if (checkMovable(start.first, teleportX(start.second-1)) &&
                        checkMovable(teleportY(start.first+1), teleportX(start.second-1))) {
                    currDirection = KEY_LEFT;
                }
            }
            break;
        default:
            // something wrong ... cannot find shortest path ...
            currDirection = 0;
            break;
    }

    return currDirection;
}

std::vector<Node> Bot::FillFindNeighbor(Node curr, int status) {
    std::vector<Node> result;
    int locY, locX;

    locY = teleportY(curr.first - 1);
    locX = curr.second;
    if (visited[locY][locX] == status)
        result.push_back(Node(locY, locX));

    locY = teleportY(curr.first + 1);
    if (visited[locY][locX] == status)
        result.push_back(Node(locY, locX));

    locY = curr.first;
    locX = teleportX(curr.second - 1);
    if (visited[locY][locX] == status)
        result.push_back(Node(locY, locX));

    locX = teleportX(curr.second + 1);
    if (visited[locY][locX] == status)
        result.push_back(Node(locY, locX));

    return result;
}

int Bot::Fill(Node &start) {
    int result = 0;

    /* TODO: can cache the path */

    // find out rounds & target
    // we can choose direction according to the spaces/rounds
    // the IMPORTANT thing is spaces should always be bigger than rounds
    // even though we still CANNOT 100% sure that snake will not die.
    // unless we generate the longest path and test for each direction
    // TODO:
    int tempRounds, rounds = INT_MAX;
    int tempSpaces, spaces = 0;
    int diff = INT_MIN;
    Node tempTarget, target = start;

    tempRounds = visited[start.first][start.second];
    tempTarget = start;
    // Up
    if ((tempSpaces = FillCheckInside(
                    Node(teleportY(start.first - 1), start.second),
                    tempRounds, tempTarget)) > tempRounds) {
        if (diff < tempSpaces - tempRounds) {
            spaces = tempSpaces;
            rounds = tempRounds;
            target = tempTarget;
            diff = tempSpaces - tempRounds;
            result = KEY_UP;
        }
    }

    tempRounds = visited[start.first][start.second];
    tempTarget = start;
    // Down
    if ((tempSpaces = FillCheckInside(
                    Node(teleportY(start.first + 1), start.second),
                    tempRounds, tempTarget)) > tempRounds) {
        if (diff < tempSpaces - tempRounds) {
            spaces = tempSpaces;
            rounds = tempRounds;
            target = tempTarget;
            diff = tempSpaces - tempRounds;
            result = KEY_DOWN;
        }
    }
    
    tempRounds = visited[start.first][start.second];
    tempTarget = start;
    // Left
    if ((tempSpaces = FillCheckInside(
                   Node(start.first, teleportX(start.second - 1)),
                   tempRounds, tempTarget)) > tempRounds) {
        if (diff < tempSpaces - tempRounds) {
            spaces = tempSpaces;
            rounds = tempRounds;
            target = tempTarget;
            diff = tempSpaces - tempRounds;
            result = KEY_LEFT;
        }
    }
    
    // Right
    tempRounds = visited[start.first][start.second];
    tempTarget = start;
    if ((tempSpaces = FillCheckInside(
                   Node(start.first, teleportX(start.second + 1)),
                   tempRounds, tempTarget)) > tempRounds) {
        if (diff < tempSpaces - tempRounds) {
            spaces = tempSpaces;
            rounds = tempRounds;
            target = tempTarget;
            diff = tempSpaces - tempRounds;
            result = KEY_RIGHT;
        }
    }

    // find neighbors of target that are visited
    std::vector<Node> targets = FillFindNeighbor(target, -1);

    // if we can find a weakest coor
    // find a long path from start to target use at least rounds movements.
    if (targets.size() > 0) {
        result = FillProcess(start, targets[0], rounds);
    }

    return result;
}


int Bot::HMTGetAction(Node &curr, Node &target) {
    // TODO: debug
    int result = 0;
    int index = hmt[curr.first][curr.second];

    if (index == -1) return result;

    int targetIndex = hmt[target.first][target.second];

    int upIndex = hmt[teleportY(curr.first-1)][curr.second];
    int downIndex = hmt[teleportY(curr.first+1)][curr.second];
    int leftIndex = hmt[curr.first][teleportX(curr.second-1)];
    int rightIndex = hmt[curr.first][teleportX(curr.second+1)];

    /* TODO: find a shortest path with combining of A* and HMT */
    if (upIndex == index + 1) {
        result = KEY_UP;
    }
    if (downIndex == index + 1) {
        result = KEY_DOWN;
    }
    if (leftIndex == index + 1) {
        result = KEY_LEFT;
    }
    if (rightIndex == index + 1) {
        result = KEY_RIGHT;
    }
    
    if (result != 0) return result;

    if (upIndex == 0) {
        result = KEY_UP;
    }
    if (downIndex == 0) {
        result = KEY_DOWN;
    }
    if (leftIndex == 0) {
        result = KEY_LEFT;
    }
    if (rightIndex == 0) {
        result = KEY_RIGHT;
    }

    return result;
};

void Bot::HMTInit() { HMTReset(); };
void Bot::HMTReset() {
    int ybound = _gameplayer->getYBound();
    int xbound = _gameplayer->getXBound();
    for (int y = 0; y < ybound; y++) {
        for (int x = 0; x < xbound; x++) {
            hmt[y][x] = -1;
        }
    }
};

bool Bot::HMTProcess(Node curr, int index) {
    // TODO: debug & Too Slow
    //if (_gameplayer->getMovableArea() == (_gameplayer->getYBound()) * (_gameplayer->getXBound())) return true;
    if (index == _gameplayer->getMovableArea()-1) {
        if (hmt[teleportY(curr.first-1)][curr.second] == 0)
            return true;
        if (hmt[teleportY(curr.first+1)][curr.second] == 0)
            return true;
        if (hmt[curr.first][teleportX(curr.second-1)] == 0)
            return true;
        if (hmt[curr.first][teleportX(curr.second+1)] == 0)
            return true;
        return false;
    }

    if (checkFence(curr)) return false;
    if (hmt[curr.first][curr.second] != -1) return false;

    hmt[curr.first][curr.second] = index;

    if (HMTProcess(Node(teleportY(curr.first-1), curr.second), index+1)) return true;
    if (HMTProcess(Node(teleportY(curr.first+1), curr.second), index+1)) return true;
    if (HMTProcess(Node(curr.first, teleportX(curr.second-1)), index+1)) return true;
    if (HMTProcess(Node(curr.first, teleportX(curr.second+1)), index+1)) return true;

    hmt[curr.first][curr.second] = -1;

    return false;
};

int Bot::HMT(Node &start, Node &end) {
    int result = HMTGetAction(start, end);

    if (result != 0)
        return result;

    HMTInit();

    if (!HMTProcess(start, 0)) return 0;

    result = HMTGetAction(start, end);
    return result;
};

int Bot::generate() {
    usleep(500);
    if (_gameplayer->checkSnake() && _gameplayer->checkFood()) {
        const Snake *snake = _gameplayer->getSnake(0);
        int direction = snake->getDirection();
        Node head = snake->getHead();
        Node tail = snake->getTail();
        Node footprint = _gameplayer->getLatestFootprint();
        Node food = _gameplayer->getFood();
        bool pathExist = false;

        switch (method) {
        case 'A':
            // force to recalculate every time
            AStarReset();

            // run A* algorithm to food
            direction = AStar(head, food);
            if (direction != 0) {
                AStarCounter = 0;
                prevDirection = direction;
                return direction;
            }

            // if we cannot find shortest path, wandering around
            direction = Wander(head, prevDirection);
            if (direction != 0) {
                AStarCounter = 0;
                prevDirection = direction;
                return direction;
            }

            // if still got stucked, counting to 500 then reset
            AStarCounter += 1;
            if (AStarCounter > 500) {
                AStarCounter = 0;
                return 'r';
            }

            return prevDirection;
        case 'a':
            // force to recalculate every time
            AStarReset();

            // run A* algorithm to food
            direction = AStar(head, food);
            if (direction != 0) {
                AStarCounter = 0;
                prevDirection = direction;
                return direction;
            }

            // if still got stucked, counting to 500 then reset
            AStarCounter += 1;
            if (AStarCounter > 500) {
                AStarCounter = 0;
                return 'r';
            }

            return prevDirection;
        case 'h':
            // build and search through Hamiltonian Cycle
            direction = HMT(head, food);
            if (direction != 0) {
                HMTCounter = 0;
                prevDirection = direction;
                return direction;
            }

            // if still got stucked, counting to 500 then reset
            HMTCounter += 1;
            if (HMTCounter > 500) {
                HMTCounter = 0;
                return 'r';
            }

            return prevDirection;
        case 'T':
            // force to recalculate every time
            AStarReset();

            // run A* algorithm to food
            direction = AStar(head, food);
            if (direction != 0) {
                AStarCounter = 0;
                prevDirection = direction;
                return direction;
            }

            // force to recalculate every time
            AStarReset();

            // if cannot find a way to food, run A* algorithm to tail
            // TODO: BUT we cannot not actully reach tail, another situation
            // is the growing snake would make its head collide with its
            // tail.
            direction = AStar(head, footprint);
            if (direction != 0) {
                AStarCounter = 0;
                prevDirection = direction;
                return direction;
            }

            // if we cannot find shortest path, wandering around
            direction = Wander(head, prevDirection);
            if (direction != 0) {
                AStarCounter = 0;
                prevDirection = direction;
                return direction;
            }

            // if still got stucked, counting to 500 then reset
            AStarCounter += 1;
            if (AStarCounter > 500) {
                AStarCounter = 0;
                return 'r';
            }

            return prevDirection;
        case 't':
            // force to recalculate every time
            AStarReset();

            // run A* algorithm to food
            direction = AStar(head, food);
            if (direction != 0) {
                AStarCounter = 0;
                prevDirection = direction;
                return direction;
            }

            // force to recalculate every time
            AStarReset();

            // if cannot find a way to food, run A* algorithm to tail
            // TODO: BUT we cannot not actully reach tail, another situation
            // is the growing snake would make its head collide with its
            // tail.
            direction = AStar(head, footprint);
            if (direction != 0) {
                AStarCounter = 0;
                prevDirection = direction;
                return direction;
            }

            // if still got stucked, counting to 500 then reset
            AStarCounter += 1;
            if (AStarCounter > 500) {
                AStarCounter = 0;
                return 'r';
            }

            return prevDirection;
        case 'O':
            debugMsg = "AStaring...";
            setDebugMsg();

            /* A* algorithm first */
            // force to recalculate every time
            AStarReset();

            // run A* algorithm to perform "head to food path-finding"
            direction = AStar(head, food);

            /* if we can find path out */
            if (direction != 0) {
                // if we don't have footprint **INITIALLY WHEN GAME START**
                // and we can find a path from head to food, use simple A* algorithm
                if (!_gameplayer->checkMovable(footprint.first, footprint.second)) return direction;

                // clear the FOOD node
                AStarReset(food.first, food.second);

                // clear all except the current path for the next "food to tail path-finding"
                AStarPreservePath = true;
                AStarReset();
                AStarPreservePath = false;

                // run A* algorithm to perform "food to tail path-finding"
                pathExist = (AStar(food, footprint) != 0);

                if (direction != 0 && pathExist) {
                    AStarCounter = 0;
                    prevDirection = direction;
                    return direction;
                }
            }

            debugMsg = "Filling...";
            setDebugMsg();

            // force to recalculate every time
            FillReset();

            /* if we can not find shortest path to food, execute fill mode */
            /* find a long enough path from start to a target snake body which is going to disappear */
            direction = Fill(head);
            if (direction != 0) {
                prevDirection = direction;
                return direction;
            }

            debugMsg = "Stucked...";
            setDebugMsg();

            // if still got stucked, counting to 500 then reset
            AStarCounter += 1;
            if (AStarCounter > 500) {
                AStarCounter = 0;
                return 'r';
            }
        case 'P':
            // force to recalculate every time
            AStarReset();

            // if cannot find a way to food, run A* algorithm to tail
            // TODO: BUT we cannot not actully reach tail, another situation
            // is the growing snake would make its head collide with its
            // tail.
            direction = AStar(head, footprint);
            if (direction != 0) {
                AStarCounter = 0;
                prevDirection = direction;
                return direction;
            }

            // if we cannot find shortest path, wandering around
            direction = Wander(head, prevDirection);
            if (direction != 0) {
                AStarCounter = 0;
                prevDirection = direction;
                return direction;
            }

            // if still got stucked, counting to 500 then reset
            AStarCounter += 1;
            if (AStarCounter > 500) {
                AStarCounter = 0;
                return 'r';
            }

            return prevDirection;
        case 'p':
            // force to recalculate every time
            AStarReset();

            // if cannot find a way to food, run A* algorithm to tail
            // TODO: BUT we cannot not actully reach tail, another situation
            // is the growing snake would make its head collide with its
            // tail.
            direction = AStar(head, footprint);
            if (direction != 0) {
                AStarCounter = 0;
                prevDirection = direction;
                return direction;
            }

            // if still got stucked, counting to 500 then reset
            AStarCounter += 1;
            if (AStarCounter > 500) {
                AStarCounter = 0;
                return 'r';
            }

            return prevDirection;
        case 'W':
            // force to recalculate every time
            AStarReset();

            // run A* algorithm
            direction = AStar(head, food);
            if (direction != 0) {
                AStarCounter = 0;
                prevDirection = direction;
                return direction;
            }

            // if we cannot find shortest path, wandering around
            direction = Wander(head, prevDirection);
            if (direction != 0) {
                AStarCounter = 0;
                prevDirection = direction;
                return direction;
            }

            // if still got stucked, counting to 500 then reset
            AStarCounter += 1;
            if (AStarCounter > 500) {
                AStarCounter = 0;
                return 'r';
            }

            return prevDirection;
        case 'F':
            debugMsg = "AStaring...";
            setDebugMsg();

            /* A* algorithm first */
            // force to recalculate every time
            AStarReset();

            // run A* algorithm
            direction = AStar(head, food);
            if (direction != 0) {
                AStarCounter = 0;
                prevDirection = direction;
                return direction;
            }

            debugMsg = "Filling...";
            setDebugMsg();

            // force to recalculate every time
            FillReset();

            /* if we can not find shortest path to food, execute fill mode */
            /* find a long enough path from start to a target snake body which is going to disappear */
            direction = Fill(head);
            if (direction != 0) {
                prevDirection = direction;
                return direction;
            }

            debugMsg = "Stucked...";
            setDebugMsg();

            // if still got stucked, counting to 500 then reset
            AStarCounter += 1;
            if (AStarCounter > 500) {
                AStarCounter = 0;
                return 'r';
            }

            return prevDirection;
        default:
            int y_head = head.first, x_head = head.second;
            int y_food = food.first, x_food = food.second;

            if (x_head > x_food)
                return KEY_LEFT;
            if (x_head < x_food)
                return KEY_RIGHT;
            if (y_head > y_food)
                return KEY_UP;
            if (y_head < y_food)
                return KEY_DOWN;

            return direction;
        };
    };
    return 0;
}

void Bot::close() {}
