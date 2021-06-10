#include <algorithm>
#include <iostream>
#include <signal.h>
#include <thread>

#include <ncurses.h>

#ifndef __BOT_HEADER__
#include "bot.h"
#endif
#ifndef __SNAKEGAMEBOARD_HEADER__
#include "snakegameboard.h"
#endif
#ifndef __SNAKEGAMEROLE_HEADER__
#include "snakegamerole.h"
#endif
#ifndef __MACRO_HEADER__
#include "macro.h"
#endif

using namespace std;

void actionHandler(SnakeGameBoard &gameboard, int aps, int &apsOffset) {
    int offset = 0;
    int usec = SECOND / max(aps + apsOffset, MINAPS);
    SnakeGameCounters *cnts = gameboard.getGameCounters();

    while (!gameboard.checkClosed()) {
        if (!gameboard.checkPaused()) {
            // do some actions here
            gameboard.placeFoodEvent();
            gameboard.moveEvent();
            gameboard.eatEvent();
            gameboard.updateGameBoard();
            cnts->increaseAction();
        }

        usec = SECOND / max(aps + apsOffset, MINAPS);
        usleep(usec);
    }
}

void counterHandler(SnakeGameBoard &gameboard, int fps, int aps, int &fpsOffset,
                    int &apsOffset) {
    int usec = SECOND;
    int updatePeriod = SECOND / usec;
    SnakeGameCounters *cnts = gameboard.getGameCounters();

    while (!gameboard.checkClosed()) {
        cnts->increaseTimer();
        cnts->updateFPS(updatePeriod);
        if ((cnts->fps > fps) && ((fps + fpsOffset) >= MINFPS))
            fpsOffset -= 1;
        if (cnts->fps < fps)
            fpsOffset += 1;
        cnts->updateAPS(updatePeriod);
        if ((cnts->aps > aps) && ((aps + apsOffset) >= MINAPS))
            apsOffset -= 1;
        if (cnts->aps < aps)
            apsOffset += 1;
        cnts->updateTPS(updatePeriod);
        cnts->updatePrev();
        usleep(usec);
    }
}

void playerViewer(SnakeGameCamera &viewer, int fps, int &fpsOffset) {
    int num = 0;
    int usec = SECOND / max(fps + fpsOffset, MINFPS);
    SnakeGameCounters *cnts = viewer.getGameCounters();

    while (!viewer.checkClosed()) {
        if (!viewer.checkPaused()) {
            viewer.updatePreparedScreen();
        }

        if (cnts->frames % fps == 0)
            viewer.scheduleRefresh();

        viewer.print();
        cnts->increaseFrame();

        /* ncurses refresh */
        refresh();

        usec = SECOND / max(fps + fpsOffset, MINFPS);
        usleep(usec);
    }
}

int waitForInput(int maxfd) {
    fd_set fds;

    FD_ZERO(&fds);
    FD_SET(fileno(stdin), &fds);

    select(maxfd + 1, &fds, NULL, NULL, NULL);
    return getch();
};

void playerController(SnakeGamePlayer &gameplayer, bool autoMode, Bot &bot) {
    int maxfd = 0, c;
    SnakeGameCounters *cnts = gameplayer.getGameCounters();

    while (!gameplayer.checkClosed()) {
        if (!autoMode) {
            c = waitForInput(maxfd);
        } else {
            // TODO: pass a full access simulator into it

            SnakeGameBoard sgbc = gameplayer.getGameBoard();
            SnakeGameCounters scnts = gameplayer.getGameCounters();
            sgbc.setGameCounters(scnts);
            sgbc.updateGameBoard();

            SnakeGamePlayer sgpc = gameplayer;
            sgpc.connectGameBoard(sgbc);

            // SnakeGameMaster simulator = gpc.getFullAccess();

            c = bot.generate();
            cnts->increaseTriggerBot();
        }

        gameplayer.control(c);
        cnts->increaseKey();
        flushinp();
    }
}

static void helpMessage() {
    cout << "syntax error." << endl
         << endl
         << "options available:" << endl
         << "-a: auto play mode, default false." << endl
         << "-e: fence around the map, default false." << endl
         << "-l: snake length, default 50." << endl
         << "-y: snake y, default 5." << endl
         << "-x: snake x, default 5." << endl
         << "-f: max frames per second, default 100. min: " << MINFPS
         << ", max: " << MAXFPS << endl
         << "-s: distance per second(speed), default 20. min: " << MINAPS
         << ", max: " << MAXAPS << endl
         << "-m: bot path-finding method. default: 'O'" << endl
         << "    'A': Pure food-finding(A*) and Wandering." << endl
         << "    'a': Pure food-finding(A*)." << endl
         << "    'T': Food-finding(A*), tail-chasing(A*) and Wandering." << endl
         << "    't': Food-finding(A*) and tail-chasing(A*)." << endl
         << "    'O': Advanced food-finding(A*) and filling(A* extension)." << endl
         << "    'P': Pure tail-chasing(A*) and Wandering." << endl
         << "    'p': Pure tail-chasing(A*)." << endl
         << "    'F': Food-finding(A*) and filling(A* extension)." << endl
         << "    'W': Wandering." << endl
         << "    others: Straight forward." << endl;
}

void cursesInit() {
    /* set curses */
    use_env;
    initscr();
    keypad(stdscr, true);
    nonl(); /* using LF instead of CR/LF */
    cbreak();
    noecho(); /* no echo input */
    curs_set(false);
    timeout(0); /* non-blocking */

    if (has_colors()) {
        start_color();

        init_pair(1, COLOR_RED, COLOR_BLACK);
        init_pair(2, COLOR_GREEN, COLOR_BLACK);
        init_pair(3, COLOR_YELLOW, COLOR_BLACK);
        init_pair(4, COLOR_BLUE, COLOR_BLACK);
        init_pair(5, COLOR_CYAN, COLOR_BLACK);
        init_pair(6, COLOR_MAGENTA, COLOR_BLACK);
        init_pair(7, COLOR_WHITE, COLOR_BLACK);
    }
}

static void finish(int sig) {
    endwin();
    exit(0);
}

/* https://invisible-island.net/ncurses/ncurses-intro.html */
int main(int argc, char *argv[]) {
    bool autoMode = false;
    bool fence = false;
    int snakelen = 50, snakey = 5, snakex = 5, fps = 100, speed = 20,
        aps = speed, fpsOffset = 0, apsOffset = 0;
    int index = 1;
    char method = 'O';
    while (index < argc) {
        if (argv[index][0] != '-') {
            helpMessage();
            return 0;
        }

        switch (argv[index][1]) {
        case 'a':
            autoMode = true;
            break;
        case 'e':
            fence = true;
            break;
        case 'l':
            snakelen = atoi(argv[++index]);
            break;
        case 'y':
            snakey = atoi(argv[++index]);
            break;
        case 'x':
            snakex = atoi(argv[++index]);
            break;
        case 'm':
            method = argv[++index][0];
            break;
        case 'f':
            fps = atoi(argv[++index]);
            if ((fps < MINFPS) || (fps > MAXFPS)) {
                helpMessage();
                return 0;
            }
            break;
        case 's':
            aps = speed = atoi(argv[++index]);
            if ((aps < MINAPS) || (aps > MAXAPS)) {
                helpMessage();
                return 0;
            }
            break;
        default:
            helpMessage();
            return 0;
        }

        ++index;
    }

    /* signal handler */
    signal(SIGINT, finish);

    /* setup curses */
    cursesInit();

    /* create objects */
    int y = LINES - 1, x = (COLS - 1) >> 1;
    SnakeGameCounters cnts;
    SnakeGameBoard gameboard(y, x, cnts);
    if (gameboard.setBackground(fence) != 0)
        finish(0);
    if (gameboard.addSnake(snakelen, snakey, snakex) != 0)
        finish(0);
    gameboard.updateGameBoard();

    SnakeGamePlayer gameplayer(0);
    SnakeGameCamera viewer(0);
    gameplayer.connectGameBoard(gameboard);
    viewer.connectGameBoard(gameboard);
    Bot bot = Bot(gameplayer, method);

    /* init threads */
    thread actionThread(actionHandler, ref(gameboard), aps, ref(apsOffset));
    thread counterThread(counterHandler, ref(gameboard), fps, speed,
                         ref(fpsOffset), ref(apsOffset));
    thread viewerThread(playerViewer, ref(viewer), fps, ref(fpsOffset));
    thread controllerThread(playerController, ref(gameplayer), autoMode,
                            ref(bot));

    /* quit, clearing thread */
    controllerThread.join();
    viewerThread.join();
    counterThread.join();
    actionThread.join();

    /* quit, clearing curses */
    bot.close();
    finish(0);
}
