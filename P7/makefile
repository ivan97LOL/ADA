# Iván Álvarez García DNI: 49623492A
OPTS = -Wall -Wextra -O3 -std=c++0x

all: maze_greedy

maze_greedy: maze_greedy.cc
	g++ ${OPTS} -o maze_greedy maze_greedy.cc	 

tar: maze_greedy.cc makefile
	tar cvfz 49623492A.tar.gz maze_greedy.cc makefile
