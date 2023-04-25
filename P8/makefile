# Iván Álvarez García DNI: 49623492A
OPTS = -Wall -Wextra -O3 -std=c++0x

all: maze_bt

maze_bt: maze_bt.cc
	g++ ${OPTS} -o maze_bt maze_bt.cc	 

tar: maze_bt.cc makefile
	tar cvfz 49623492A.tar.gz maze_bt.cc makefile
