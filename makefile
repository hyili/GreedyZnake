all: snakeutils snakegameboard bot snakegamerole snakegameagent
	g++ -std=c++11 -o output/snake -I base -I snake tmp/bot.o -pthread snake/snake.cpp -L tmp -lncurses -lsga -lsgb -lsgr -lsgu
bot:
	g++ -std=c++11 -c -o tmp/bot.o -I base -I snake snake/bot.cpp
snakegameboard:
	g++ -std=c++11 -c -o tmp/snakegameboard.o -I base -I snake snake/snakegameboard.cpp
	ar cr tmp/libsgb.a tmp/snakegameboard.o
snakegameagent: gameagent
	g++ -std=c++11 -c -o tmp/snakegameagent.o -I base -I snake snake/snakegameagent.cpp
	ar cr tmp/libsga.a tmp/gameagent.o tmp/snakegameagent.o
gameagent:
	g++ -std=c++11 -c -o tmp/gameagent.o -I base -I snake base/gameagent.cpp
snakegamerole:
	g++ -std=c++11 -c -o tmp/snakegamerole.o -I base -I snake snake/snakegamerole.cpp -lncurses
	ar cr tmp/libsgr.a tmp/snakegamerole.o
snakeutils: utils
	g++ -std=c++11 -c -o tmp/snakeutils.o -I base -I snake snake/snakeutils.cpp
	ar cr tmp/libsgu.a tmp/utils.o tmp/snakeutils.o
utils:
	g++ -std=c++11 -c -o tmp/utils.o -I base -I snake base/utils.cpp


debug: Dsnakeutils Dsnakegameboard Dbot Dsnakegamerole Dsnakegameagent
	g++ -std=c++11 -g -o output/snake -I base -I snake tmp/bot.o -pthread snake/snake.cpp -L tmp -lncurses -lsga -lsgb -lsgr -lsgu
Dbot:
	g++ -std=c++11 -g -c -o tmp/bot.o -I base -I snake snake/bot.cpp
Dsnakegameboard:
	g++ -std=c++11 -g -c -o tmp/snakegameboard.o -I base -I snake snake/snakegameboard.cpp
	ar cr tmp/libsgb.a tmp/snakegameboard.o
Dsnakegameagent: Dgameagent
	g++ -std=c++11 -g -c -o tmp/snakegameagent.o -I base -I snake snake/snakegameagent.cpp
	ar cr tmp/libsga.a tmp/gameagent.o tmp/snakegameagent.o
Dgameagent:
	g++ -std=c++11 -g -c -o tmp/gameagent.o -I base -I snake base/gameagent.cpp
Dsnakegamerole:
	g++ -std=c++11 -g -c -o tmp/snakegamerole.o -I base -I snake snake/snakegamerole.cpp -lncurses 
	ar cr tmp/libsgr.a tmp/snakegamerole.o
Dsnakeutils: Dutils
	g++ -std=c++11 -g -c -o tmp/snakeutils.o -I base -I snake snake/snakeutils.cpp
	ar cr tmp/libsgu.a tmp/utils.o tmp/snakeutils.o
Dutils:
	g++ -std=c++11 -g -c -o tmp/utils.o -I base -I snake base/utils.cpp

clear: clean
	rm output/*
clean:
	rm tmp/*


# link order issue
# https://stackoverflow.com/questions/2624238/c-undefined-references-with-static-library 
