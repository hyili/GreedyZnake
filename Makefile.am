AUTOMAKE_OPTIONS = subdir-objects
AM_CXXFLAGS = -std=c++11 -I base -I snake_src

bin_PROGRAMS = snake
snake_SOURCES = snake_src/snake.cpp snake_src/bot.cpp
snake_LDADD = libgbf.a libsnake.a
snake_LDFLAGS = -lncurses -lpthread

noinst_LIBRARIES = libgbf.a libsnake.a
libgbf_a_SOURCES = base/gameagent.cpp base/utils.cpp
libsnake_a_SOURCES = snake_src/snakegameagent.cpp snake_src/snakegameboard.cpp snake_src/snakegamerole.cpp snake_src/snakeutils.cpp


#AUTOMAKE_OPTIONS = subdir-objects
#AM_CXXFLAGS = -std=c++11 -I base -I snake_src
#
#bin_PROGRAMS = snake
#snake_SOURCES = snake_src/snake.cpp snake_src/bot.cpp
#snake_LDADD = libsga.a libsgg.a libsgr.a libsgu.a 
#snake_LDFLAGS = -lncurses -lpthread
#
#noinst_LIBRARIES = libsga.a libsgg.a libsgr.a libsgu.a
#libsga_a_SOURCES = base/gameagent.cpp snake_src/snakegameagent.cpp
#libsgg_a_SOURCES = snake_src/snakegameboard.cpp
#libsgr_a_SOURCES = snake_src/snakegamerole.cpp
#libsgu_a_SOURCES = base/utils.cpp snake_src/snakeutils.cpp
